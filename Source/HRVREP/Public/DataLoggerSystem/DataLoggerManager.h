// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataLoggerSystem/TextFileManager.h"
#include "DataLoggerManager.generated.h"



UCLASS(Blueprintable)
class HRVREP_API UDataLoggerManager : public UObject
{
	GENERATED_BODY()


public:	

	// Sets default values for this component's properties
	UDataLoggerManager();

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "Initialise"))
	// Set the log directory
	virtual void Init(const FString& InDirectoryPath, const FString& InFileName);

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "Directory"))
	// Set the log directory
	virtual void SetDirectory(const FString& InDirectoryPath);

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "File Name"))
	// Set the file name
	virtual void SetFileName(const FString& InFileName);

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "Header"))
	// Set the file name
	virtual void SetDataHeader(const FString& InHeader);

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "Log New Data"))
	// Log new string data
	virtual void LogNewData(const FString& NewString);

	UFUNCTION(Blueprintcallable, Category = "DataLoggerManager", meta = (Keywords = "Save Close"))
	// Close the logger
	virtual void CloseLogger();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

private:
	FString DataHeader;
	FString DataLog;
	FString SaveFileName;
	FString SaveDirectory;
	FTextFileManager* TextFileManager;


		
};
