// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


FTextFileManager::FTextFileManager() : FilePath(), Thread(nullptr), ContentQueue(), FileWriter(nullptr), RunningFlag(false)
{

}

FTextFileManager::FTextFileManager(const FString& InFilePath) : FilePath(InFilePath),Thread(nullptr), ContentQueue(), FileWriter(nullptr), RunningFlag(false)
{
	Thread = TUniquePtr<FRunnableThread>(FRunnableThread::Create(this, *FString::Printf(TEXT("FileWriterThread_%s"), *FPaths::GetBaseFilename(FilePath)), 0, TPri_Normal));

	FileWriter = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*FilePath));
}

FTextFileManager::~FTextFileManager()
{
	// Stop the thread first
	Stop();

	//
	if (Thread)
	{
		Thread->WaitForCompletion();
	}

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
}

/*

// Save a FString to text file through file stream, more efficient
bool SaveStringTextStream(const FString& SaveDirectory, const FString& FileName, const FString& SaveText, bool AllowOverWrite)
{
	// Check if overwrite is allowed
	if (!AllowOverWrite)
		return false; // Currently only overwrite the file


	// Set complete file path
	FString CompleteFilePath = SaveDirectory + "\\" + FileName;

	TUniquePtr<FArchive> FileWriter(IFileManager::Get().CreateFileWriter(*CompleteFilePath));

	if (FileWriter)
	{
		// Convert FString to UTF-8 bytes
		FTCHARToUTF8 UTF8String(SaveText);
		FileWriter->Serialize((void*)UTF8String.Get(), UTF8String.Length());

		FileWriter->Close();
	}
}
*/
