// Copyright Epic Games, Inc. All Rights Reserved.

#include "OceanEngine.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogOceanEngine, Log, All);

void FOceanEngineModule::StartupModule()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("OceanEngine"));

	if (Plugin.IsValid())
	{
		const FString ShaderDirectory = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/OceanEngine"), ShaderDirectory);
		UE_LOG(LogOceanEngine, Log, TEXT("Shader directory registered: %s"), *ShaderDirectory);
	}
	else
	{
		UE_LOG(LogOceanEngine, Error, TEXT("Could not find the OceanEngine plugin."));
	}
}

void FOceanEngineModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FOceanEngineModule, OceanEngine)
