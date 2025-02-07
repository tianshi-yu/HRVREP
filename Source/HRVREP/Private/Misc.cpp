// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc.h"
FString UMisc::GetTimeNow()
{
	FDateTime CurrentUTCTime = FDateTime::Now();
	int32 Milliseconds = CurrentUTCTime.GetMillisecond();

	FString FormattedUTC = FString::Printf(TEXT("%s.%03d"), *CurrentUTCTime.ToString(TEXT("%Y-%m-%d %H:%M:%S")), Milliseconds);
	return FormattedUTC;
}
