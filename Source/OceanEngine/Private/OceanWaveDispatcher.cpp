#include "OceanWaveDispatcher.h"
#include "OceanWaveShader.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TextureResource.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"

void UOceanWaveDispatcher::DispatchOceanWaves(
	UTextureRenderTarget2D* RenderTarget,
	float Time,
	const FOceanWaveSettings& Settings)
{
	if (!IsValid(RenderTarget))
	{
		return;
	}

	if (!RenderTarget->bSupportsUAV)
	{
		UE_LOG(LogTemp, Error, TEXT("OceanEngine: Render target must have UAV enabled."));
		return;
	}

	FTextureRenderTargetResource* Resource = RenderTarget->GameThread_GetRenderTargetResource();
	if (Resource == nullptr)
	{
		return;
	}

	const FIntPoint Size(RenderTarget->SizeX, RenderTarget->SizeY);

	ENQUEUE_RENDER_COMMAND(OceanEngineDispatchWaves)(
		[Resource, Size, Time, Settings](FRHICommandListImmediate& RHICmdList)
		{
			FRHITexture* Texture = Resource->GetRenderTargetTexture();
			if (Texture == nullptr)
			{
				return;
			}

			TShaderMapRef<FOceanWaveCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
			if (!ComputeShader.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("OceanEngine: shader not compiled. Check OceanWave.usf and the log."));
				return;
			}

			FRDGBuilder GraphBuilder(RHICmdList);

			FRDGTextureRef OutputTexture = RegisterExternalTexture(
				GraphBuilder, Texture, TEXT("OceanEngine.Output"));

			auto* Parameters = GraphBuilder.AllocParameters<FOceanWaveCS::FParameters>();
			Parameters->OutputTexture = GraphBuilder.CreateUAV(OutputTexture);
			Parameters->Time          = Time;
			Parameters->Amplitude     = Settings.Amplitude;
			Parameters->Wavelength    = Settings.Wavelength;
			Parameters->Speed         = Settings.Speed;
			Parameters->TileSize      = Settings.TileSize;

			const FIntVector GroupCount(
				FMath::DivideAndRoundUp(Size.X, 8),
				FMath::DivideAndRoundUp(Size.Y, 8),
				1);

			FComputeShaderUtils::AddPass(
				GraphBuilder,
				RDG_EVENT_NAME("OceanEngine.Waves"),
				ComputeShader,
				Parameters,
				GroupCount);

			GraphBuilder.Execute();
		});
}
