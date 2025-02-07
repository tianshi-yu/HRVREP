// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SensorSystem/SensorManager.h"
#include "ChaosVehicleMovementComponent.h"
#include "TruckPawnSensorManager.generated.h"

/**
 * Sensor manager class of the truck pawn
 */
UCLASS(Blueprintable)
class HRVREP_API UTruckPawnSensorManager : public UObject, public ISensorManager
{
	GENERATED_BODY()

public:
	// Initialise a sensor
	virtual void InitSensor(const FString& InName, float InRate) override;

	// Initialise a sensor, overloading to pass the vehicle component
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Init"))
	virtual void InitSensor(const FString& InName, float InRate, UChaosVehicleMovementComponent* InVehicle);

	// Set the name of the sensor
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Set Name"))
	virtual void SetSensorName(const FString& InName) override; 

	// Get the name of the sensor
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Get Name"))
	virtual const FString& GetSensorName() const override; 

	// Set the sampling rate of the sensor
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Set Sampling Rate"))
	virtual void SetSensorSamplingRate(float InRate) override; 

	// Get the sampling rate of the sensor
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Get Sampling Rate"))
	virtual float GetSensorSamplingRate() const override; 

	// Update the sensor readings, to be called in ticks
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Update"))
	virtual void UpdateReadings() override;

	// Retreive the readings as a TMap using varibale names as keys
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retrieve Readings"))
	virtual const TMap<FString, double>& RetrieveReadings() const override;

	// Retrive the variable names as array
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retrieve Var Name"))
	virtual TArray<FString> RetrieveVarNames() const override;

	// Retrieve the values as array
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retrieve Var Values"))
	virtual TArray<double> RetrieveVarValues() const override;

	// Retrieve a specific values by variable name
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retreive Var Values Name"))
	virtual double RetrieveVarValuesByName(const FString& InName) const override;

	// Retrive all the varible names in a single string
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retrieve Var Name String"))
	virtual FString RetrieveVarNamesStr(const FString& Delimiter) const override;

	// Retrieve all the variable values in a single string
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Retrieve Var Value String"))
	virtual FString RetrieveVarValuesStr(const FString& Delimiter) const override;

	// Set the vehicle component
	UFUNCTION(Blueprintcallable, Category = "TruckPawnSensorManager", meta = (Keywords = "Set Vehicle"))
	virtual void SetVehicleComponent(UChaosVehicleMovementComponent* InVehicle);


private:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	float SamplingRate;

	UPROPERTY()
	TMap<FString, double> Readings;
	
	UPROPERTY()
	UChaosVehicleMovementComponent* Vehicle;
	
};
