// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathSSE.h"
#include "Math/Vector.h"
#include "UObject/ObjectMacros.h"
#include "UserDataTypes.generated.h"

/**
 * Struct header file store user data types, you can define your own, the User Subsystem use the generic one
 */

USTRUCT(BlueprintType)
struct FGenericUserData
{
	GENERATED_BODY()

public:
	FGenericUserData()
		: FirstName("Default")
		, SurName("Default")
		, YearOfBirth(1995)
		, Height(175)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Data")
	FString FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Data")
	FString SurName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Data")
	int YearOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Data")
	int Height;

};