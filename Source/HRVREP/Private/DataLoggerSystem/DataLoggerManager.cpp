// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoggerSystem/DataLoggerManager.h"

//
// Default constructor
//
UDataLoggerManager::UDataLoggerManager(): DataHeader(), DataLog(), SaveFileName(), SaveDirectory(), TextFileManager(nullptr)
{
	
}

//
// Initialise the data logger
//
void UDataLoggerManager::Init(const FString& DirectoryPath, const FString& FileName) 
{
	SetDirectory(DirectoryPath);
	SetFileName(FileName);
	TextFileManager = new FTextFileManager(FPaths::Combine(DirectoryPath, FileName));
}

//
// Set the log directory
//
void UDataLoggerManager::SetDirectory(const FString& DirectoryPath)
{
	this->SaveDirectory = DirectoryPath;
}

//
// Set the file name
//
void UDataLoggerManager::SetFileName(const FString& FileName)
{
	this->SaveFileName = FileName;
}

//
// Set the file name
//
void UDataLoggerManager::SetDataHeader(const FString& InHeader)
{
	this->DataHeader = InHeader;

}


// Log new string data
void UDataLoggerManager::LogNewData(const FString& NewString)
{
	TextFileManager->NewContent(NewString + LINE_TERMINATOR);
}

// Save log
void UDataLoggerManager::CloseLogger()
{
	TextFileManager->Stop();
}




