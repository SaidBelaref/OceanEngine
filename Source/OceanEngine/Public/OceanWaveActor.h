#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OceanWaveDispatcher.h"
#include "OceanWaveActor.generated.h"

class UTextureRenderTarget2D;

/** Drop this in the level, assign a render target, and the waves animate by themselves. */
UCLASS()
class OCEANENGINE_API AOceanWaveActor : public AActor
{
	GENERATED_BODY()

public:
	AOceanWaveActor();

	/** RGBA16f render target with "Enable UAV" checked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	UTextureRenderTarget2D* RenderTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanEngine")
	FOceanWaveSettings Settings;

	virtual void Tick(float DeltaSeconds) override;

	// Also animates in the editor viewport, without pressing Play
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

private:
	float AccumulatedTime = 0.f;
};
