// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSystem/UserSubSystem.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "LogCategories.h"

void UUserSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	

	FString CombineSaveDirectory = FPaths::Combine(FPaths::ProjectDir(), "SavedUserData");
	this->PersistentSaveDirectory = CombineSaveDirectory;
	UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: UserSubSystem Initialized"));
	UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: Persistent Save Directory %s"), *PersistentSaveDirectory);
	Super::Initialize(Collection);

}

void UUserSubSystem::Deinitialize()
{
	
	UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: UserSubSystem Deinitialized"));
	Super::Deinitialize();
	
}

//
// Set user data save directory, use InDirectory to specify the path, "" will use the persistent path
//
void UUserSubSystem::SetUserDirectory(const FString& InDirectory)
{
	if (InDirectory == "")
	{
		FString UserSpecifcPath = FString::Printf(TEXT("USER_%c%c%d%d"),
			this->UserData.FirstName[0],
			this->UserData.SurName[0],
			this->UserData.YearOfBirth,
			this->UserData.Height);
		FString GenerateDirectoryPath = FPaths::Combine(this->PersistentSaveDirectory,UserSpecifcPath);
		
		this->UserDirectory = GenerateDirectoryPath;

	}
	else
	{
		this->UserDirectory = InDirectory;
	}

	UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: User Save Directory Set to '%s'."), *UserDirectory);
	
}

//
// Save user data, use InDirectory to specify the path, "" will use the persistent path
//
void UUserSubSystem::SaveUserData(const FString& InDirectory)
{
	FString OutJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(UserData, OutJsonString);
	if (InDirectory == "")
	{

		FFileHelper::SaveStringToFile(OutJsonString,*FPaths::Combine(UserDirectory,"UserInfo.json"));
		UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: User Data Saved to User Directory '%s'."), *UserDirectory);
	}
	else
	{
		FFileHelper::SaveStringToFile(OutJsonString, *FPaths::Combine(InDirectory, "UserInfo.json"));
		UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: User Data Saved to Customed Directory '%s'."), *InDirectory);
	}
	
}

//
// Load user data with the ID (user folder name), use InDirectory to specify the path, "" will use the persistent path
//
void UUserSubSystem::LoadUserData(const FString& InUserID, const FString& InDirectory)
{
	FString JsonString;
	FString Path;
	if (InDirectory == "")
	{
		Path = FPaths::Combine(PersistentSaveDirectory, InUserID, "UserInfo.json");
	}
	else
	{
		Path = FPaths::Combine(InDirectory, InUserID, "UserInfo.json");
	}

	if(FFileHelper::LoadFileToString(JsonString, *Path))
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserData);
		UE_LOG(LogUserSubSystem, Warning, TEXT("Subsystem Log: User Data of '%s' is loaded."), *UserData.FirstName);
	}
	else
	{
		UE_LOG(LogUserSubSystem, Error, TEXT("Subsystem Log: Failed to load the JSON file."));
	}

	// Finally set the new save directory
	SetUserDirectory(InDirectory);
}

