#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OceanWaveDispatcher.generated.h"

class UTextureRenderTarget2D;

USTRUCT(BlueprintType)
struct OCEANENGINE_API FOceanWaveSettings
{
	GENERATED_BODY()

	/** Height of the largest wave (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	float Amplitude = 50.f;

	/** Approximate base wavelength (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	float Wavelength = 1000.f;

	/** Animation speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	float Speed = 1.f;

	/** World size (cm) covered by the texture; it tiles seamlessly */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	float TileSize = 4000.f;
};

UCLASS()
class OCEANENGINE_API UOceanWaveDispatcher : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Runs the wave compute shader into a UAV-enabled RGBA16f render target. */
	UFUNCTION(BlueprintCallable, Category = "OceanEngine")
	static void DispatchOceanWaves(
		UTextureRenderTarget2D* RenderTarget,
		float Time,
		const FOceanWaveSettings& Settings);
};
