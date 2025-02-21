// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/RadarSensorManager.h"

void URadarSensorManager::UpdateReadings()
{
	Readings.Add("Distance-" + Name, RadarSensorActor->GetData().Distance);
	Readings.Add("Bearing-" + Name, RadarSensorActor->GetData().Bearing);
}

void URadarSensorManager::InitRadarSensor(const FString& InName, float InRate, ARadarSensorActor* InRadarSensorActor, FRadarConfig Config)
{
	Name = InName;
	SamplingRate = InRate;
	RadarSensorActor = InRadarSensorActor;
	RadarSensorActor->Configure(Config);
}
