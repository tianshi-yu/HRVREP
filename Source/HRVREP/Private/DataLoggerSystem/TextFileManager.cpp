// Fill out your copyright notice in the Description page of Project Settings.


#include "DataLoggerSystem/TextFileManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


FTextFileManager::FTextFileManager() : FilePath(), Thread(nullptr), ContentQueue(), FileWriter(nullptr), RunningFlag(false)
{

}

FTextFileManager::FTextFileManager(const FString& InFilePath) : FilePath(InFilePath),Thread(nullptr), ContentQueue(), FileWriter(nullptr), RunningFlag(false)
{
	Thread = TUniquePtr<FRunnableThread>(FRunnableThread::Create(this, *FString::Printf(TEXT("FileWriterThread_%s"), *FPaths::GetBaseFilename(FilePath)), 0, TPri_Normal));

	FileWriter = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*FilePath));

	UE_LOG(LogTemp, Log, TEXT("Text file created at: %s"), *FilePath);
}

FTextFileManager::~FTextFileManager()
{
	UE_LOG(LogTemp, Log, TEXT("Text file created at: %s is closed."), *FilePath);
}

void FTextFileManager::NewContent(const FString& InContent)
{
	this->ContentQueue.Enqueue(InContent);
}

bool FTextFileManager::Init()
{
	RunningFlag = true;

	return true;  // Initialization success
}

uint32 FTextFileManager::Run()
{
	
	while (RunningFlag)
	{

		if (FileWriter)
		{
			FString Content;
			if (ContentQueue.Dequeue(Content))
			{
				// Convert FString to UTF-8 bytes
				FTCHARToUTF8 UTF8String(Content);
				FileWriter->Serialize((void*)UTF8String.Get(), UTF8String.Length());

			}
		}
	}
	FileWriter->Close(); // In the end close the file writer

	return 0;  // Task is complete
}

void FTextFileManager::Stop()
{
	RunningFlag = false;
	if (Thread)
	{
		Thread->WaitForCompletion();
	}
}

