// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SensorSystem/GenericSensorManager.h"
#include "ChaosVehicleMovementComponent.h"
#include "TruckPawnSensorManager.generated.h"

/**
 * Sensor manager class of the truck pawn
 */
UCLASS(Blueprintable, ClassGroup = (CustomSensor), meta = (BlueprintSpawnableComponent))
class HRVREP_API UTruckPawnSensorManager : public UGenericSensorManager
{
	GENERATED_BODY()

public:
	// Initialise a sensor
	virtual void InitSensor(const FString& InName, float InRate) override;

	// Initialise a sensor, overloading to pass the vehicle component
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Init"))
	virtual void InitTruckPawnSensor(const FString& InName, float InRate, UChaosVehicleMovementComponent* InVehicle);

	// Update the sensor readings, to be called in ticks
	virtual void UpdateReadings() override;

	// Set the vehicle component
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Set Vehicle"))
	virtual void SetVehicleComponent(UChaosVehicleMovementComponent* InVehicle);


private:
	
	UPROPERTY(BlueprintSetter = SetVehicleComponent, Category = "TruckPawnSensorManager")
	UChaosVehicleMovementComponent* Vehicle; // Life cycle of vehicle is managed by the engine
	
};
