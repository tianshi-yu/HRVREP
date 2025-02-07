// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/GenericSensorManager.h"

//
// Initialise a sensor
//
void UGenericSensorManager::InitSensor(const FString& InName, float InRate)
{
	SetSensorName(InName);
	SetSensorSamplingRate(InRate);
}

//
// Set the name of the sensor
//
void UGenericSensorManager::SetSensorName(const FString& InName)
{
	this->Name = InName;
}

//
// Get the name of the sensor
//
const FString& UGenericSensorManager::GetSensorName() const
{
	return Name;
}

//
// Set the sampling rate of the sensor
//
void UGenericSensorManager::SetSensorSamplingRate(float InRate)
{
	this->SamplingRate = InRate;
}

//
// Get the sampling rate of the sensor
//
float UGenericSensorManager::GetSensorSamplingRate() const
{
	return SamplingRate;
}

//
// Update the sensor readings, to be called in ticks
//
void UGenericSensorManager::UpdateReadings()
{
}

//
// Retreive the readings as a TMap using varibale names as keys
//
const TMap<FString, double>& UGenericSensorManager::RetrieveReadings() const
{
	return Readings;
}

//
// Retrive the variable names as array
//
TArray<FString> UGenericSensorManager::RetrieveVarNames() const
{
	TArray<FString> VarNames;
	Readings.GenerateKeyArray(VarNames);
	return VarNames;
}

//
// Retrieve the values as array
//
TArray<double> UGenericSensorManager::RetrieveVarValues() const
{

	TArray<double> VarValues;
	Readings.GenerateValueArray(VarValues);
	return VarValues;
}

//
// Retrieve a specific values by variable name
//
double UGenericSensorManager::RetrieveVarValuesByName(const FString& InName) const
{
	return Readings[InName];
}

//
// Retrive all the varible names in a single string
//
FString UGenericSensorManager::RetrieveVarNamesStr(const FString& Delimiter) const
{
	TArray<FString> VarNames;
	Readings.GenerateKeyArray(VarNames);


	FString VarNamesStr;
	// Convert double to FString
	for (const FString Value : VarNames)
	{
		VarNamesStr.Append(Value + Delimiter);
	}


	/*
	FString VarNamesStr = FString::Join(VarNames, *Delimiter); // Join is more efficient
	*/

	return VarNamesStr;
}

//
// Retrieve all the variable values in a single string
//
FString UGenericSensorManager::RetrieveVarValuesStr(const FString& Delimiter) const
{
	TArray<double> VarValues;
	Readings.GenerateValueArray(VarValues);


	FString VarValuesStr;
	for (const double Value : VarValues)
	{
		VarValuesStr.Append(FString::SanitizeFloat(Value) + Delimiter);
	}

	/*
	TArray<FString> VarValuesStrArray;

	// Convert each double to FString
	for (double Value : VarValues)
	{
		VarValuesStrArray.Add(FString::SanitizeFloat(Value));
	}

	FString VarValuesStr = FString::Join(VarValuesStrArray, *Delimiter); // Join is more efficient
	*/

	return VarValuesStr;
}