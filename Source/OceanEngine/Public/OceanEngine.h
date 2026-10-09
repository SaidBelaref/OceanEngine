// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FOceanEngineModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	 * Singleton-like access to this module's interface.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FOceanEngineModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FOceanEngineModule>("OceanEngine");
	}

	/**
	 * Checks to see if this module is already loaded and ready to go.
	 *
	 * @return True if the module is in memory and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("OceanEngine");
	}
};
