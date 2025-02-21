// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/RadarSensorActor.h"

// Sets default values
ARadarSensorActor::ARadarSensorActor()
{
	
}

// Called when the game starts or when spawned
void ARadarSensorActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARadarSensorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Config the sensor from owner object
void ARadarSensorActor::Configure(const FRadarConfig& InRadarConfig)
{
	SetConfig(InRadarConfig);
	RadarData.Distance = InRadarConfig.TraceLength;
	UE_LOG(LogTemp, Display, TEXT("Radar configed: FOV: %f, number of trace: %d, max trace length: %f."), RadarConfig.FOV, RadarConfig.TraceBearings.Num(),RadarConfig.TraceLength);
}

const FRadarData& ARadarSensorActor::GetData() const
{
	return RadarData;
}

void ARadarSensorActor::SetData(const FRadarData& InRadarData)
{
	RadarData.Distance = InRadarData.Distance;
	RadarData.Bearing = InRadarData.Bearing;
}

void ARadarSensorActor::SetConfig(const FRadarConfig& InRadarConfig)
{
	RadarConfig.FOV = InRadarConfig.FOV;
	RadarConfig.TraceLength = InRadarConfig.TraceLength;
	RadarConfig.TraceNumber = InRadarConfig.TraceNumber;
	RadarConfig.TraceBearings = CalculateTraceBearings(InRadarConfig);
}

const FRadarConfig& ARadarSensorActor::GetConfig() const
{
	return RadarConfig;
}

/*
const FString& ARadarSensorActor::GetName() const
{
	return RadarName;
}

void ARadarSensorActor::SetName(const FString& InName)
{
	RadarName = InName;
}
*/

TArray<float> ARadarSensorActor::CalculateTraceBearings(const FRadarConfig& InRadarConfig)
{
	float BearingLimit = InRadarConfig.FOV / 2.0f;
	float BearingInterval = InRadarConfig.FOV / (1.0f * InRadarConfig.TraceNumber);

	TArray<float> Bearings;
	for (float Value = -BearingLimit; Value <= BearingLimit; Value += BearingInterval)
	{
		Bearings.Add(Value);
	}
	UE_LOG(LogTemp, Display, TEXT("Radar trace bearing limit: %f , interval: %f, number of trace: %d"), BearingLimit, BearingInterval, Bearings.Num());
	return Bearings;
}

