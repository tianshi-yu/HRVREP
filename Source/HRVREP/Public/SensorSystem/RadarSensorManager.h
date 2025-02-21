// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SensorSystem/GenericSensorManager.h"
#include "SensorSystem/RadarSensorActor.h"
#include "RadarSensorManager.generated.h"

/**
 * Sensor Manager Class for Distance Radars
 */



UCLASS(Blueprintable, ClassGroup = (CustomSensor), meta = (BlueprintSpawnableComponent))
class HRVREP_API URadarSensorManager : public UGenericSensorManager
{
	GENERATED_BODY()

public:

	// Update sensor readings override the function
	virtual void UpdateReadings() override;


	// Init
	UFUNCTION(BlueprintCallable, Category = "RadarSensorManager", meta = (Keywords = "Init"))
	virtual void InitRadarSensor(const FString& InName, float InRate, ARadarSensorActor* InRadarSensorActor, FRadarConfig Config);


private:
	UPROPERTY(EditAnywhere, Category = "RadarSensorManager")
	ARadarSensorActor* RadarSensorActor;
	

};
