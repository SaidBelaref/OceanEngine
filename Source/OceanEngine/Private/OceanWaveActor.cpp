#include "OceanWaveActor.h"
#include "Components/SceneComponent.h"

AOceanWaveActor::AOceanWaveActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOceanWaveActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AccumulatedTime += DeltaSeconds;

	UOceanWaveDispatcher::DispatchOceanWaves(RenderTarget, AccumulatedTime, Settings);
}
