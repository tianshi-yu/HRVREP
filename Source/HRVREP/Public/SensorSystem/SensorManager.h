// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SensorManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USensorManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface class for all types of sensors.
 * Important Note: To use the pointer, you need to use the Unreal smart pointer: TScriptInterface<ISensorManager> instead of ISensorManager*
 */
class HRVREP_API ISensorManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Initialise a sensor
	virtual void InitSensor(const FString& InName, float InRate) = 0; //pure virtual

	// Set the name of the sensor
	virtual void SetSensorName(const FString& InName)  = 0; //pure virtual

	// Get the name of the sensor
	virtual const FString& GetSensorName() const = 0; //pure virtual

	// Set the sampling rate of the sensor
	virtual void SetSensorSamplingRate(float InRate) = 0; //pure virtual

	// Get the sampling rate of the sensor
	virtual float GetSensorSamplingRate() const = 0; //pure virtual

	// Get the variable names as array
	virtual TArray<FString> RetrieveVarNames() const = 0; //pure virtual

	// Retreive the readings as a TMap using varibale names as keys
	virtual const TMap<FString, double>& RetrieveReadings() const = 0; //pure virtual

	// Retrieve the values as array
	virtual TArray<double> RetrieveVarValues() const = 0; //pure virtual

	// Retrieve a specific values by variable name
	virtual double RetrieveVarValuesByName(const FString& InName) const = 0; //pure virtual

	// Retrive all the varible names in a single string
	virtual FString RetrieveVarNamesStr(const FString& Delimiter) const = 0; //pure virtual

	// Retrieve all the variable values in a single string
	virtual FString RetrieveVarValuesStr(const FString& Delimiter) const = 0; //pure virtual

	// Update the sensor readings, to be called in ticks
	virtual void UpdateReadings() = 0; //pure virtual

};
