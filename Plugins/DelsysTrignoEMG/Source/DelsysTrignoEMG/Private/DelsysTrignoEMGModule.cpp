// Copyright Epic Games, Inc. All Rights Reserved.

#include "DelsysTrignoEMGModule.h"


//#define LOCTEXT_NAMESPACE "FDelsysTrignoEMGModule"

DEFINE_LOG_CATEGORY(LogDelsysTrignoEMG);

void FDelsysTrignoEMGModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG Plugin has started!"));
}

void FDelsysTrignoEMGModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG Plugin has shut down!"));
}

IMPLEMENT_MODULE(FDelsysTrignoEMGModule, DelsysTrignoEMG)


//#undef LOCTEXT_NAMESPACE
	
