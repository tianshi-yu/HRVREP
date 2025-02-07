// Fill out your copyright notice in the Description page of Project Settings.


#include "UserSystem/UserSubSystem.h"

void UUserSubSystem::SetUserDirectory(const FString& NewDirectory)
{
	if (NewDirectory == "")
	{
		FString UserSpecifcPath = FString::Printf(TEXT("%c%c%d%d"),
			this->FirstName[0],
			this->SurName[0],
			this->YearOfBirth,
			this->Height);
		FString GenerateDirectoryPath = FPaths::Combine(FPaths::ProjectDir(), "SavedUserData",UserSpecifcPath);
		
		this->UserDirectory = GenerateDirectoryPath;

	}
	else
	{
		this->UserDirectory = NewDirectory;
	}

	UE_LOG(LogTemp, Warning, TEXT("User Save Directory Set to '%s'."), *UserDirectory);
	
}

