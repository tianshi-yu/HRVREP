// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UserSystem/UserDataTypes.h"
#include "DataLoggerSystem/TextFileManager.h"
#include "UserSubSystem.generated.h"

/**
 * Basic user system class, derive from this class for other type of users
 */
UCLASS()
class HRVREP_API UUserSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 
	//******************************* User Data **************************************//
	//

	
	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Get User Data"))
	FGenericUserData GetUserData() const { return UserData; };

	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Get User Data"))
	void SetUserData(FGenericUserData InUserData) { UserData = InUserData; };
	

	// 
	//******************************* System Operations **************************************//
	//
	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Get Directory"))
	const FString& GetUserDirectory() const { return UserDirectory; }

	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Set Directory"))
	void SetUserDirectory(const FString& NewDirectory);

	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Save User Data"))
	void SaveUserData(const FString& NewDirectory);

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetUserData, BlueprintSetter = SetUserData)
	FGenericUserData UserData;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetUserDirectory, BlueprintSetter = SetUserDirectory)
	FString UserDirectory;

};
