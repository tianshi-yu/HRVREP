// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/TruckPawnSensorManager.h"
#include "ChaosWheeledVehicleMovementComponent.h"

//
// Initialise a sensor
//
void UTruckPawnSensorManager::InitSensor(const FString& InName, float InRate)
{
	Super::InitSensor(InName, InRate);
	SetVehicleComponent(Vehicle);
	UE_LOG(LogTemp, Warning, TEXT("Truck Pawn Sensor: use InitTruckPawnSensor function with vehicle component argument!"));
	// overloading
}

//
// Initialise a sensor, overloading to pass the vehicle component
//
void UTruckPawnSensorManager::InitTruckPawnSensor(const FString& InName, float InRate, UChaosVehicleMovementComponent* InVehicle)
{
	SetSensorName(InName);
	SetSensorSamplingRate(InRate);
	SetVehicleComponent(InVehicle);
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
	
	Readings.Add("LocX-" + Name, Vehicle->GetActorLocation().X);
	Readings.Add("LocY-" + Name, Vehicle->GetActorLocation().Y);
	Readings.Add("LocZ-" + Name, Vehicle->GetActorLocation().Z);
	Readings.Add("FwdDirX-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().X);
	Readings.Add("FwdDirY-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().Y);
	Readings.Add("FwdDirZ-" + Name, Vehicle->GetActorTransform().GetRotation().GetForwardVector().Z);
	Readings.Add("OrientW-" + Name, Vehicle->GetActorTransform().GetRotation().W);
	Readings.Add("OrientX-" + Name, Vehicle->GetActorTransform().GetRotation().X);
	Readings.Add("OrientY-" + Name, Vehicle->GetActorTransform().GetRotation().Y);
	Readings.Add("OrientZ-" + Name, Vehicle->GetActorTransform().GetRotation().Z);
	Readings.Add("Speed-" + Name, Vehicle->GetForwardSpeedMPH()* 1.60934);// MPH to KMH
	Readings.Add("Throttle-" + Name, Vehicle->GetThrottleInput());
	Readings.Add("Brake-" + Name, Vehicle->GetBrakeInput());
	Readings.Add("Steering-" + Name, Vehicle->GetSteeringInput());
	Readings.Add("Gear-" + Name, Vehicle->GetCurrentGear());
	Readings.Add("EngineRPM-" + Name, WheeledVehicle->GetEngineRotationSpeed());
	
	
	
}









