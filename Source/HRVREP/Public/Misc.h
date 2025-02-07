// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc.generated.h"

/**
 * 
 */
UCLASS()
class HRVREP_API UMisc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get current machine time, return as a formated string
	UFUNCTION(Blueprintcallable, Category = "Misc", meta = (Keywords = "Time"))
	static FString GetTimeNow();
	
};
