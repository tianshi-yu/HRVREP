// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DelsysTrignoEMGModule.h"
#include "TextFileManager.h"
#include "DelsysTrignoEMG.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (DelsysTrignoEMGSensor), meta = (BlueprintSpawnableComponent))
class DELSYSTRIGNOEMG_API UDelsysTrignoEMG : public UActorComponent
{
	GENERATED_BODY()

public:
    // Enum to represent sensor types
    enum class SensorTypes
    {
        SensorTrigno,
        SensorTrignoImu,
        SensorTrignoMiniHead,
        NoSensor
    };

    UDelsysTrignoEMG();
    ~UDelsysTrignoEMG();

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Connect"))
    bool Connect();

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Close"))
    void Close();

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Start Acquisition"))
    void StartAcquisition();

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Stop Acquisition"))
    void StopAcquisition();

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Start Recording"))
    void StartRecording(const FString& FilePath);

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Stop Recording"))
    void StopRecording();

  

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Get EMG"))
    TArray<float> GetRawEMGData() const;

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Connected"))
    bool IsConnected() const { return bConnected; }

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Acquiring"))
    bool IsAcquiring() const { return bAcquiring; }

    UFUNCTION(Blueprintcallable, Category = "DelsysEMG", meta = (Keywords = "Recording"))
    bool IsRecording() const { return bRecording; }


private:
    // Send command to Trigno Control Utility server
    //TFuture<FString> SendCommand(FString Command);
    FString SendCommand(FString Command);

    // Acquire data
    void AcquireData();

    // Dictionary (Map) for sensor string-to-enum mapping
    TMap<FString, SensorTypes> SensorTypeDict;

    // List to keep track of various sensor types
    TArray<SensorTypes> SensorTypeList;

    // Active sensor channels
    TArray<int32> ActiveSensorChannels;

    // Temporary EMG data buffer
    TArray<float> TempEMGDataList;

    // Temporary IMU data buffer
    TArray<float> TempIMUDataList;

    // TCP Sockets
    TSharedPtr<FSocket> CommandSocket;
    TSharedPtr<FSocket> EMGDataSocket;
    TSharedPtr<FSocket> IMUDataSocket;

    // Ports
    const int32 COMMAND_PORT = 50040;
    const int32 EMG_DATA_PORT = 50043;
    const int32 IMU_DATA_PORT = 50044;

    // Commands
    const FString COMMAND_QUIT =  "QUIT";
    const FString COMMAND_GETTRIGGERS = "TRIGGER?";
    const FString COMMAND_SETSTARTTRIGGER = "TRIGGER START";
    const FString COMMAND_SETSTOPTRIGGER = "TRIGGER STOP";
    const FString COMMAND_START = "START";
    const FString COMMAND_STOP = "STOP";
    const FString COMMAND_SENSOR_TYPE = "TYPE?";

    // Status Flags
    bool bConnected = false;
    bool bInitialized = false;
    bool bAcquiring = false;
    bool bRecording = false;
    

    // Saving File
    FString EMGFileName;
    FString IMUFileName;
    FTextFileManager* EMGFileManager;
    FTextFileManager* IMUFileManager;

    // Sensor data acquisition worker thread
    class FTrignoAcquisitionThread : public FRunnable
    {
    private:
        TSharedPtr<UDelsysTrignoEMG> Owner;
        TUniquePtr<FRunnableThread> Thread;
        bool bRunning;

    public:
        FTrignoAcquisitionThread(UDelsysTrignoEMG* InOwner) : Owner(InOwner), bRunning(true)
        {
            Thread = TUniquePtr<FRunnableThread>(FRunnableThread::Create(this, TEXT("EMGThread"), 0, TPri_Normal));
        }

        virtual uint32 Run() override
        {
            bRunning = true;
            while (bRunning)
            {
                if (Owner && Owner->bAcquiring)
                {
                    Owner->AcquireData();
                }
            }
            return 0;
        }

        void Stop() { bRunning = false; }
        ~FTrignoAcquisitionThread() 
        { 
            Stop();  
            if (Thread) { Thread->WaitForCompletion(); } 
        }
    };

    TUniquePtr<FTrignoAcquisitionThread> TrignoAcquisitionThreadInstance;

	
};
