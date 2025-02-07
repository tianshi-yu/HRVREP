// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UserSubSystem.generated.h"

/**
 * Basic user system class, derive from this class for other type of users
 */
UCLASS()
class HRVREP_API UUserSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetFirstName, BlueprintSetter = SetFirstName)
	FString FirstName;
	UPROPERTY(EditAnywhere, BlueprintGetter = GetSurName, BlueprintSetter = SetSurName)
	FString SurName;
	UPROPERTY(EditAnywhere, BlueprintGetter = GetYearOfBirth, BlueprintSetter = SetYearOfBirth)
	int YearOfBirth;
	UPROPERTY(EditAnywhere, BlueprintGetter = GetHeight, BlueprintSetter = SetHeight)
	int Height;
	UPROPERTY(EditAnywhere, BlueprintGetter = GetUserDirectory, BlueprintSetter = SetUserDirectory)
	FString UserDirectory;


public:
	UFUNCTION(BlueprintGetter, Category = "UserSubSystem", meta = (Keywords = "Get First Name"))
	const FString& GetFirstName() const { return FirstName; }

	UFUNCTION(BlueprintSetter, Category = "UserSubSystem", meta = (Keywords = "Set First Name"))
	void SetFirstName(const FString& NewName) { FirstName = NewName; }

	UFUNCTION(BlueprintGetter, Category = "UserSubSystem", meta = (Keywords = "Get Sur Name"))
	const FString& GetSurName() const { return SurName; }

	UFUNCTION(BlueprintSetter, Category = "UserSubSystem", meta = (Keywords = "Set Sur Name"))
	void SetSurName(const FString& NewName) { SurName = NewName; }


	UFUNCTION(BlueprintGetter, Category = "UserSubSystem", meta = (Keywords = "Get Year Birth"))
	int GetYearOfBirth() const { return YearOfBirth; }

	UFUNCTION(BlueprintSetter, Category = "UserSubSystem", meta = (Keywords = "Set Sur Name"))
	void SetYearOfBirth(int value) { YearOfBirth = value; }


	UFUNCTION(BlueprintGetter, Category = "UserSubSystem", meta = (Keywords = "Get Height"))
	int GetHeight() const { return Height; }

	UFUNCTION(BlueprintSetter, Category = "UserSubSystem", meta = (Keywords = "Set Height"))
	void SetHeight(int value) { Height = value; }

	UFUNCTION(BlueprintGetter, Category = "UserSubSystem", meta = (Keywords = "Get Directory"))
	const FString& GetUserDirectory() const { return UserDirectory; }

	UFUNCTION(BlueprintSetter, Category = "UserSubSystem", meta = (Keywords = "Set Directory"))
	void SetUserDirectory(const FString& NewDirectory);


	
};
