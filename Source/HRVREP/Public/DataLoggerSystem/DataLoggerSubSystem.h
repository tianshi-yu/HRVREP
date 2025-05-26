// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataLoggerSystem/DataLoggerManager.h"
#include "DataLoggerSubSystem.generated.h"



/**
 * Data logger subsystem, operating all data loggers to save data to local files.
 */

UCLASS()
class HRVREP_API UDataLoggerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End USubsystem
	virtual void Deinitialize() override;
	
	// Add a datalogger
	UFUNCTION(Blueprintcallable, Category = "DataLoggerSubSystem", meta = (Keywords = "Add"))
	virtual bool AddDataLogger(const FString& LoggerName, const FString& DirectoryPath);
	// Remove a datalogger
	UFUNCTION(Blueprintcallable, Category = "DataLoggerSubSystem", meta = (Keywords = "Remove"))
	virtual bool RemoveDataLogger(const FString& LoggerName);
	// End a datalogger and save
	UFUNCTION(Blueprintcallable, Category = "DataLoggerSubSystem", meta = (Keywords = "End Save"))
	virtual bool EndDataLogger(const FString& LoggerName);

	// Find the datalogger by name
	UFUNCTION(Blueprintcallable, Category = "DataLoggerSubSystem", meta = (Keywords = "Find"))
	virtual UDataLoggerManager* FindDataLoggerByName(const FString& LoggerName) const;



private:
	UPROPERTY()
	TMap<FString,UDataLoggerManager*> DataLoggerMap;
	
};
