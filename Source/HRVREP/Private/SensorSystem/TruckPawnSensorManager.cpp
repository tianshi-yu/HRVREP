// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/TruckPawnSensorManager.h"
#include "ChaosWheeledVehicleMovementComponent.h"

//
// Initialise a sensor
//
void UTruckPawnSensorManager::InitSensor(const FString& InName, float InRate)
{
	UE_LOG(LogTemp, Warning, TEXT("Truck Pawn Sensor: use initilisation function with vehicle component argument!"));
	// overloading
}

//
// Initialise a sensor, overloading to pass the vehicle component
//
void UTruckPawnSensorManager::InitSensor(const FString& InName, float InRate, UChaosVehicleMovementComponent* InVehicle)
{
	SetSensorName(InName);
	SetSensorSamplingRate(InRate);
	SetVehicleComponent(InVehicle);
}

//
// Set the name of the sensor
//
void UTruckPawnSensorManager::SetSensorName(const FString& InName)
{
	this->Name = InName;
}

//
// Get the name of the sensor
//
const FString& UTruckPawnSensorManager::GetSensorName() const
{
	return Name;
}

//
// Set the sampling rate of the sensor
//
void UTruckPawnSensorManager::SetSensorSamplingRate(float InRate)
{
	this->SamplingRate = InRate;
}

//
// Get the sampling rate of the sensor
//
float UTruckPawnSensorManager::GetSensorSamplingRate() const
{
	return SamplingRate;
}


//
// Set the vehicle component
//
void UTruckPawnSensorManager::SetVehicleComponent(UChaosVehicleMovementComponent* InVehicle)
{
	this->Vehicle = InVehicle;
}


//
// Update the sensor readings, to be called in ticks
// Main updates
//
void UTruckPawnSensorManager::UpdateReadings()
{
	// Some information require the wheeled vehicle component
	UChaosWheeledVehicleMovementComponent* WheeledVehicle = Cast<UChaosWheeledVehicleMovementComponent>(Vehicle); 

	Readings.Add("LocationX-" + Name, Vehicle->GetActorLocation().X);
	Readings.Add("LocationY-" + Name, Vehicle->GetActorLocation().Y);
	Readings.Add("LocationZ-" + Name, Vehicle->GetActorLocation().Z);
	Readings.Add("ForwardDirX-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().X);
	Readings.Add("ForwardDirY-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().Y);
	Readings.Add("ForwardDirZ-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().Z);
	Readings.Add("Speed-" + Name, Vehicle->GetForwardSpeedMPH()* 1.60934);// MPH to KMH
	Readings.Add("Throttle-" + Name, Vehicle->GetThrottleInput());
	Readings.Add("Brake-" + Name, Vehicle->GetBrakeInput());
	Readings.Add("Steering-" + Name, Vehicle->GetSteeringInput());
	Readings.Add("Gear-" + Name, Vehicle->GetCurrentGear());
	Readings.Add("EngineRPM-" + Name, WheeledVehicle->GetEngineRotationSpeed());
	

	
}

//
// Retreive the readings as a TMap using varibale names as keys
//
const TMap<FString, double>& UTruckPawnSensorManager::RetrieveReadings() const
{
	return Readings;
}

//
// Retrive the variable names as array
//
TArray<FString> UTruckPawnSensorManager::RetrieveVarNames() const
{
	TArray<FString> VarNames;
	Readings.GenerateKeyArray(VarNames);
	return VarNames;
}

//
// Retrieve the values as array
//
TArray<double> UTruckPawnSensorManager::RetrieveVarValues() const
{

	TArray<double> VarValues;
	Readings.GenerateValueArray(VarValues);
	return VarValues;
}

//
// Retrieve a specific values by variable name
//
double UTruckPawnSensorManager::RetrieveVarValuesByName(const FString& InName) const
{
	return Readings[InName];
}

//
// Retrive all the varible names in a single string
//
FString UTruckPawnSensorManager::RetrieveVarNamesStr(const FString& Delimiter) const
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
FString UTruckPawnSensorManager::RetrieveVarValuesStr(const FString& Delimiter) const
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








