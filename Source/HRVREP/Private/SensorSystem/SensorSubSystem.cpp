// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorSystem/SensorSubSystem.h"
#include "LogCategories.h"

//
// Initialise the sensor system
//
void USensorSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogSensorSubSystem, Warning, TEXT("Subsysmtem Log: SensorSubSystem Initialized"));
}

//
//  Deinitialise the sensor system
//
void USensorSubSystem::Deinitialize()
{
	UE_LOG(LogSensorSubSystem, Warning, TEXT("Subsysmtem Log: SensorSubSystem Deinitialized"));
	Super::Deinitialize();
}

//
// Add a sensor object
//
bool USensorSubSystem::AddSensor(TScriptInterface<ISensorManager> Sensor)
{
	FString Name = Sensor->GetSensorName();
	if (Name.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsysmtem Log: Sensor needs to have a name!!!"));
		return false;
	}
	else
	{
		SensorMap.Add(Name, Sensor);
		return true;
	}
}


//
// Remove a sensor object
//
bool USensorSubSystem::RemoveSensor(const FString& SensorName)
{
	if (SensorMap.Contains(SensorName))
	{
		SensorMap.Remove(SensorName);
		return true;
	}
	else
	{
		return false;
	}
}

//
// Find the sensor by name
//
TScriptInterface<ISensorManager> USensorSubSystem::FindSensorByName(const FString& SensorName) const
{
	TScriptInterface<ISensorManager> Target = SensorMap[SensorName];
	return Target;
}

//
// Update All sensor readings
//
bool USensorSubSystem::UpdateAllReadings()
{
	if (SensorMap.IsEmpty())
	{
		UE_LOG(LogSensorSubSystem, Warning, TEXT("Subsysmtem Log: There is no sensor in the system!!!"));
		return false;
	}
	else
	{
		for (auto& Element : SensorMap)
		{
			Element.Value->UpdateReadings();
		}
		return true;
	}
}

//
// Retrieve the varible names of all sensors
//
FString USensorSubSystem::RetrieveAllVarNamesStr(const FString& Delimiter)
{
	if (SensorMap.IsEmpty())
	{
		UE_LOG(LogSensorSubSystem, Warning, TEXT("Subsysmtem Log: There is no sensor in the system!!!"));
		return "";
	}
	else
	{
		FString VarNames;
		for (auto& Element : SensorMap)
		{
			VarNames.Append(Element.Value->RetrieveVarNamesStr(Delimiter));
		}
		VarNames.LeftChopInline(Delimiter.Len());
		return VarNames;

	}
}

//
// Retrieve the latest varible values of all sensors
//
FString USensorSubSystem::RetrieveAllVarValuesStr(const FString& Delimiter)
{
	if (SensorMap.IsEmpty())
	{
		UE_LOG(LogSensorSubSystem, Warning, TEXT("Subsysmtem Log: There is no sensor in the system!!!"));
		return "";
	}
	else
	{
		FString VarValues;
		for (auto& Element : SensorMap)
		{
			VarValues.Append(Element.Value->RetrieveVarValuesStr(Delimiter));
		}
		VarValues.LeftChopInline(Delimiter.Len());
		return VarValues;
	}
	
}

