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
	//******************************* Override **************************************//
	//

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

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

	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Get Persistent Directory"))
	const FString& GetPersistentSaveDirectory() const { return PersistentSaveDirectory; }

	// Set user data save directory, use InDirectory to specify the path, "" will use the persistent path
	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Set  Directory"))
	void SetUserDirectory(const FString& InDirectory);

	// Save user data, use InDirectory to specify the path, "" will use the persistent path
	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Save User Data"))
	void SaveUserData(const FString& InDirectory);

	// Load user data with the ID (user folder name), use InDirectory to specify the path, "" will use the persistent path
	UFUNCTION(BlueprintCallable, Category = "UserSubSystem", meta = (Keywords = "Load User Data"))
	void LoadUserData(const FString& InUserID, const FString& InDirectory);


private:

	UPROPERTY(BlueprintGetter = GetUserData, BlueprintSetter = SetUserData)
	FGenericUserData UserData;

	UPROPERTY(BlueprintGetter = GetUserDirectory, BlueprintSetter = SetUserDirectory)
	FString UserDirectory;

	UPROPERTY(BlueprintGetter = GetPersistentSaveDirectory)
	FString PersistentSaveDirectory;

};
