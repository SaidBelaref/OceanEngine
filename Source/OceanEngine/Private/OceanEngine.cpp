// Copyright Epic Games, Inc. All Rights Reserved.

#include "OceanEngine.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FOceanEngineModule"

DEFINE_LOG_CATEGORY_STATIC(LogOceanEngine, Log, All);

void FOceanEngineModule::StartupModule()
{
	UE_LOG(LogOceanEngine, Log, TEXT("OceanEngine module started"));
}

void FOceanEngineModule::ShutdownModule()
{
	UE_LOG(LogOceanEngine, Log, TEXT("OceanEngine module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOceanEngineModule, OceanEngine)
