// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"


class FTextFileManager : public FRunnable
{
private:
	FString FilePath;
	TUniquePtr<FRunnableThread> Thread;
	TQueue<FString> ContentQueue;
	TUniquePtr<FArchive> FileWriter;
	bool RunningFlag;

public:
	FTextFileManager();
	FTextFileManager(const FString& InFilePath);
	virtual ~FTextFileManager() override;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;


	virtual void NewContent(const FString& InContent);
};


