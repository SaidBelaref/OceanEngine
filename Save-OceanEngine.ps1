
$ErrorActionPreference = 'Stop'

# OceanEngine plugin location and expected GitHub repository
$PluginPath = 'C:\UE5_Projects\UE5_Projects\WaterSimu\Plugins\OceanEngine'
$ExpectedRemote = 'https://github.com/SaidBelaref/OceanEngine.git'

function Invoke-Git {
    param([string[]]$GitArgs)

    & git @GitArgs
    if ($LASTEXITCODE -ne 0) {
        throw "Git command failed: git $($GitArgs -join ' ')"
    }
}

try {
    if (-not (Test-Path -LiteralPath $PluginPath -PathType Container)) {
        throw "Plugin folder not found: $PluginPath"
    }

    Set-Location -LiteralPath $PluginPath

    # Make sure we are at the Git repository root.
    $Prefix = & git rev-parse --show-prefix
    if ($LASTEXITCODE -ne 0) {
        throw 'This folder is not inside a valid Git repository.'
    }
    if (-not [string]::IsNullOrWhiteSpace(($Prefix -join ''))) {
        throw 'Run this script from the plugin repository root.'
    }

    # Verify the intended remote before uploading anything.
    $Remote = & git remote get-url origin
    if ($LASTEXITCODE -ne 0 -or $Remote.Trim() -ne $ExpectedRemote) {
        throw 'The origin URL does not match the expected OceanEngine repository.'
    }

    Write-Host "`nChecking GitHub for newer commits..." -ForegroundColor Cyan
    Invoke-Git -GitArgs @('fetch', 'origin', 'main')

    # Stop if GitHub has commits that this local branch does not contain.
    $Counts = & git rev-list --left-right --count 'origin/main...HEAD'
    if ($LASTEXITCODE -ne 0) {
        throw 'Could not compare the local and remote histories.'
    }

    $Parts = ($Counts -join ' ').Trim() -split '\s+'
    $RemoteAhead = [int]$Parts[0]
    $LocalAhead = [int]$Parts[1]

    if ($RemoteAhead -gt 0) {
        throw 'GitHub has newer commits. No commit or push was attempted. Synchronize the repository before trying again.'
    }

    # Stage plugin changes. .gitignore excludes generated folders.
    $Status = & git status --porcelain
    if ($LASTEXITCODE -ne 0) {
        throw 'Could not inspect the working tree.'
    }

    if ($Status.Count -gt 0 -and -not [string]::IsNullOrWhiteSpace(($Status -join ''))) {
        Invoke-Git -GitArgs @('add', '-A')

        Write-Host "`nChanges prepared for commit:" -ForegroundColor Cyan
        Invoke-Git -GitArgs @('diff', '--cached', '--stat')

        $Answer = Read-Host "`nCommit these changes and push to GitHub? Type YES to continue"
        if ($Answer -cne 'YES') {
            Write-Host 'Cancelled. No commit or push was performed.' -ForegroundColor Yellow
            exit 0
        }

        $Message = 'OceanEngine backup ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
        Invoke-Git -GitArgs @('commit', '-m', $Message)
        $LocalAhead++
    }
    elseif ($LocalAhead -eq 0) {
        Write-Host "`nNothing new to back up. Local branch and GitHub are synchronized." -ForegroundColor Green
        exit 0
    }
    else {
        Write-Host "`nThere are local commits waiting to be uploaded." -ForegroundColor Yellow
        $Answer = Read-Host 'Push these commits to GitHub? Type YES to continue'
        if ($Answer -cne 'YES') {
            Write-Host 'Cancelled. No push was performed.' -ForegroundColor Yellow
            exit 0
        }
    }

    Write-Host "`nUploading to GitHub..." -ForegroundColor Cyan
    Invoke-Git -GitArgs @('push', 'origin', 'main')

    Write-Host "`nBackup completed successfully!" -ForegroundColor Green
    Invoke-Git -GitArgs @('status', '--short', '--branch')
}
catch {
    Write-Host "`nBACKUP STOPPED: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host 'Review the message above before trying again.' -ForegroundColor Yellow
    exit 1
}
