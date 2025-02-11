// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/ViveXRHeadsetSensorManager.h"
#include "Kismet/GameplayStatics.h"
//
// Update the sensor readings, to be called in ticks
//
void UViveXRHeadsetSensorManager::UpdateReadings()
{
	// Use VIVE OpenXR API to acquire data
	FEyeTrackerGazeData GazeData;
	UEyeTrackerFunctionLibrary::GetGazeData(GazeData);

	FXrGazeDataHTC GazeValidData = UViveOpenXREyeTrackerFunctionLibrary::GetHTCEyeGazeValidData();
	FXrPupilDataHTC PupilData = UViveOpenXREyeTrackerFunctionLibrary::GetHTCPupilData();
	FXrEyeGeometricDataHTC GazeGeoData = UViveOpenXREyeTrackerFunctionLibrary::GetHTCEyeGeometricData();


	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerCameraManager* ActiveCamera = PlayerController->PlayerCameraManager;
	FVector HMDLocation = ActiveCamera->GetCameraLocation();
	FQuat HMDRotation = ActiveCamera->GetCameraRotation().Quaternion();



	
	// Add to readings
	// HMD
	Readings.Add("LocX-" + Name, HMDLocation.X);
	Readings.Add("LocY-" + Name, HMDLocation.Y);
	Readings.Add("LocZ-" + Name, HMDLocation.Z);
	Readings.Add("OrientW-" + Name, HMDRotation.W);
	Readings.Add("OrientX-" + Name, HMDLocation.X);
	Readings.Add("OrientY-" + Name, HMDLocation.Y);
	Readings.Add("OrientZ-" + Name, HMDLocation.Z);
	Readings.Add("FwdDirX-" + Name, HMDRotation.GetForwardVector().X);
	Readings.Add("FwdDirY-" + Name, HMDRotation.GetForwardVector().Y);
	Readings.Add("FwdDirZ-" + Name, HMDRotation.GetForwardVector().Z);
	// Eye Tracking
	Readings.Add("ValidREye-" + Name, GazeValidData.rightEyeGazeValid);
	Readings.Add("ValidLEye-" + Name, GazeValidData.leftEyeGazeValid);
	Readings.Add("GazeOriginX-" + Name, GazeData.GazeOrigin.X);
	Readings.Add("GazeOriginY-" + Name, GazeData.GazeOrigin.Y);
	Readings.Add("GazeOriginZ-" + Name, GazeData.GazeOrigin.Z);
	Readings.Add("GazeDirtX-" + Name, GazeData.GazeDirection.X);
	Readings.Add("GazeDirtY-" + Name, GazeData.GazeDirection.Y);
	Readings.Add("GazeDirtZ-" + Name, GazeData.GazeDirection.Z);
	// Pupil
	Readings.Add("ValidREyePupil-" + Name, PupilData.rightEyeDiameterValid);
	Readings.Add("RPupilDiam-" + Name, PupilData.rightEyePupilDiameter);
	Readings.Add("ValidLEyePupil-" + Name, PupilData.leftEyeDiameterValid);
	Readings.Add("LPupilDiam-" + Name, PupilData.leftEyePupilDiameter);
	// Eye Geometry
	Readings.Add("ValidREyeGeom-" + Name, GazeGeoData.rightEyeIsValid);
	Readings.Add("REyeOpeness-" + Name, GazeGeoData.rightEyeOpenness);
	Readings.Add("REyeSqueeze-" + Name, GazeGeoData.rightEyeSqueeze);
	Readings.Add("REyeWide-" + Name, GazeGeoData.rightEyeWide);
	Readings.Add("ValidLEyeGeom-" + Name, GazeGeoData.leftEyeIsValid);
	Readings.Add("LEyeOpeness-" + Name, GazeGeoData.leftEyeOpenness);
	Readings.Add("LEyeSqueeze-" + Name, GazeGeoData.leftEyeSqueeze);
	Readings.Add("LEyeWide-" + Name, GazeGeoData.leftEyeWide);



}

