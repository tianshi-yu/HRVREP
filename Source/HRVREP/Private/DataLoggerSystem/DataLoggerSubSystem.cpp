// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoggerSystem/DataLoggerSubSystem.h"
#include "Misc/Paths.h"

void UDataLoggerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Subsystem Log: DataLoggerSubSystem Initialized"));
}

void UDataLoggerSubSystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Subsystem Log: DataLoggerSubSystem Deinitialized"));
	Super::Deinitialize();
}

//
// Add a datalogger
//
bool UDataLoggerSubSystem::AddDataLogger(const FString& LoggerName, const FString& DirectoryPath)
{
	
	if (this->DataLoggerMap.Contains(LoggerName)) // Check for duplicate name
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem Log: Data Logger name '%s' already exists!"), *LoggerName);
		return false; // Decline addition
	}
	
	UDataLoggerManager* NewDataLogger = NewObject<UDataLoggerManager>(this);
	if (DirectoryPath == "")
	{
		// If not specificed save in the saved directory of the project
		NewDataLogger->Init(FPaths::ProjectSavedDir(), LoggerName + ".csv");
	}
	else
	{
		// Or use the specified path
		NewDataLogger->Init(DirectoryPath, LoggerName + ".csv");
	}

	
	DataLoggerMap.Add(LoggerName, NewDataLogger);
	UE_LOG(LogTemp, Display, TEXT("Subsystem Log: Added Logger: %s"), *LoggerName);
	return true;
}

//
// Remove a datalogger
//
bool UDataLoggerSubSystem::RemoveDataLogger(const FString& LoggerName)
{
	if (DataLoggerMap.Contains(LoggerName))
	{
		DataLoggerMap.Remove(LoggerName);
		return true;
	}
	else
	{
		return false;
	}	
}

//
//End a datalogger and remove from list
//
bool UDataLoggerSubSystem::EndDataLogger(const FString& LoggerName)
{
	if (DataLoggerMap.Contains(LoggerName))
	{
		DataLoggerMap[LoggerName]->CloseLogger();
		return RemoveDataLogger(LoggerName);
	}
	{
		return false;
	}

}

//
// Find the datalogger by name
//
UDataLoggerManager* UDataLoggerSubSystem::FindDataLoggerByName(const FString& LoggerName) const
{
	UDataLoggerManager* Target = DataLoggerMap[LoggerName];
	// Commented out the log, this function will be frequently called
	//UE_LOG(LogTemp, Warning, TEXT("Find Data Logger with Name: %s"), *LoggerName);
	return Target;
};




