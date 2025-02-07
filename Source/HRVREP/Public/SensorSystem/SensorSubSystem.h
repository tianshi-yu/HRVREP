// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SenSorSystem/SensorManager.h"
#include "SensorSubSystem.generated.h"

/**
 * Sub system managing the adding removing retrieving information of the sensor in the platform
 */
UCLASS()
class HRVREP_API USensorSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End USubsystem
	virtual void Deinitialize() override;

	// Add a sensor object
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Add"))
	virtual bool AddSensor(TScriptInterface<ISensorManager> Sensor);
	// Remove a sensor object
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Remove"))
	virtual bool RemoveSensor(const FString& SensorName);
	// Find the sensor by name
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Find"))
	virtual TScriptInterface<ISensorManager> FindSensorByName(const FString& SensorName) const;


	// Update All sensor readings
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Update"))
	virtual bool UpdateAllReadings();

	// Retrieve the varible names of all sensors
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Retrieve Name String"))
	virtual FString RetrieveAllVarNamesStr(const FString& Delimiter);
	// Retrieve the latest varible values of all sensors
	UFUNCTION(Blueprintcallable, Category = "SensorSubSystem", meta = (Keywords = "Retrieve Value String"))
	virtual FString RetrieveAllVarValuesStr(const FString& Delimiter);
	



private:
	UPROPERTY()
	TMap<FString, TScriptInterface<ISensorManager>> SensorMap;
};
