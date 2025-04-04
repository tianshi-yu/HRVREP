// Copyright Epic Games, Inc. All Rights Reserved.
// This plugin is made for acquiring and saving data from Delsys Trigno EMG sensor system
// Trigno Control Utility SDK is required

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDelsysTrignoEMG, Log, All);

class FDelsysTrignoEMGModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
