// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSystem/UserSubSystem.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

void UUserSubSystem::SetUserDirectory(const FString& NewDirectory)
{
	if (NewDirectory == "")
	{
		FString UserSpecifcPath = FString::Printf(TEXT("%c%c%d%d"),
			this->UserData.FirstName[0],
			this->UserData.SurName[0],
			this->UserData.YearOfBirth,
			this->UserData.Height);
		FString GenerateDirectoryPath = FPaths::Combine(FPaths::ProjectDir(), "SavedUserData",UserSpecifcPath);
		
		this->UserDirectory = GenerateDirectoryPath;

	}
	else
	{
		this->UserDirectory = NewDirectory;
	}

	UE_LOG(LogTemp, Warning, TEXT("User Save Directory Set to '%s'."), *UserDirectory);
	
}

void UUserSubSystem::SaveUserData(const FString& NewDirectory)
{
	FString OutJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(UserData, OutJsonString);
	if (NewDirectory == "")
	{

		FFileHelper::SaveStringToFile(OutJsonString,*FPaths::Combine(UserDirectory,"UserInfo.Json"));
		UE_LOG(LogTemp, Warning, TEXT("User Data Saved to User Directory '%s'."), *UserDirectory);
	}
	else
	{
		FFileHelper::SaveStringToFile(OutJsonString, *FPaths::Combine(NewDirectory, "UserInfo.Json"));
		UE_LOG(LogTemp, Warning, TEXT("User Data Saved to Customed Directory '%s'."), *NewDirectory);
	}
	
}

