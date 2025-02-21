// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SensorSystem/GenericSensorManager.h"
#include "ViveOpenXREyeTrackerFunctionLibrary.h"
#include "HTCEyeTrackerTypes.h"
#include "EyeTrackerFunctionLibrary.h"
#include "ChaosVehicleMovementComponent.h"
#include "ViveXRHeadsetSensorManager.generated.h"

/**
 * Sensor manager class of the VIVE XR Elite Headset and attached VIVE Full Face tracker
 */
UCLASS(Blueprintable, ClassGroup = (CustomSensor), meta = (BlueprintSpawnableComponent))
class HRVREP_API UViveXRHeadsetSensorManager : public UGenericSensorManager 
{
	GENERATED_BODY()
public:

	// Update sensor readings override the function
	virtual void UpdateReadings() override;

};
