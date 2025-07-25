// Fill out your copyright notice in the Description page of Project Settings.


#include "DelsysTrignoEMG.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

UDelsysTrignoEMG::UDelsysTrignoEMG()
{
    SensorTypeList.Reserve(16);
    SensorTypeDict.Add("A", SensorTypes::SensorTrigno);
    SensorTypeDict.Add("D", SensorTypes::SensorTrigno);
    SensorTypeDict.Add("L", SensorTypes::SensorTrignoImu);
    SensorTypeDict.Add("J", SensorTypes::SensorTrignoMiniHead);
    SensorTypeDict.Add("O", SensorTypes::SensorTrignoImu);
}


UDelsysTrignoEMG::~UDelsysTrignoEMG()
{

}



bool UDelsysTrignoEMG::Connect()
{
    // Instantiate the command socket
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    CommandSocket = MakeShareable(SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DelsysCommandSocket"), false));
    CommandSocket->SetNonBlocking(false); // Blocking for reques-reply mode
    CommandSocket->SetReuseAddr(true);

    // IP address and port
    FIPv4Address IP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), IP);
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    Addr->SetPort(COMMAND_PORT);

    

    // Connect to server
    if (CommandSocket->Connect(*Addr))
    {
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Connected to Delsys Trigno Control Utility server."));
        bConnected = true;
    }
    else
    {
        UE_LOG(LogDelsysTrignoEMG, Error, TEXT("Failed to connect to Delsys Trigno Control Utility server!"));
        bConnected = false;
    }

    // Use async task to setup the sensor list and sensor configs
    // Avoid block the main game thread
    Async(EAsyncExecution::Thread, [this]()
        {
            if (bConnected)
            {
                FString Command = LINE_TERMINATOR;
                FString Response = SendCommand(Command);

                // Build a list of connected sensor types
                for (int i = 1; i <= MAX_SENSOR; i++)
                {
                    Command = FString::Printf(TEXT("SENSOR %d %s"), i, *COMMAND_SENSOR_TYPE);
                    Response;
                    Response = SendCommand(Command);

                    SensorTypeList.Add(Response.Contains("INVALID") ? SensorTypes::NoSensor : SensorTypes::SensorTrignoImu);
                }

                // Record the active sensor channel number and data channels for AUX port
                for (int i = 0; i < MAX_SENSOR; i++)
                {
                    if (SensorTypeList[i] == SensorTypes::SensorTrignoImu)
                    {
                        ActiveSensorList.SensorChannels.Add(i);
                        Command = FString::Printf(TEXT("SENSOR %d EMGCHANNELCOUNT ? "), i + 1); // How many EMG data channels
                        Response = SendCommand(Command); 
                        float Number = FCString::Atof(*Response);
                        ActiveSensorList.EMGDataChannels.Add(Number);

                        Command = FString::Printf(TEXT("SENSOR %d AUXCHANNELCOUNT ? "), i + 1); // How many AUX data channels
                        Response = SendCommand(Command);
                        Number = FCString::Atof(*Response);
                        ActiveSensorList.AUXDataChannels.Add(Number);

                        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys sensor %d is active."), i + 1);
                    }
                }

                // Config sensors
                // Backwards compatibility
                Command = TEXT("BACKWARDS COMPATIBILITY ON"); // To lock the streaming sampling rate
                Response = SendCommand(Command);
                Command = TEXT("BACKWARDS COMPATIBILITY?"); // To lock the streaming sampling rate
                Response = SendCommand(Command);
                if (Response.Contains("Yes"))
                    ServerBackwardsComatability = true;

                // Upsample On
                Command = TEXT("UPSAMPLE OFF");
                Response = SendCommand(Command);
                Command = TEXT("UPSAMPLING?"); // To lock the streaming sampling rate
                Response = SendCommand(Command);
                if (Response.Contains("Yes"))
                    ServerUpSampling = true;

                // Frame interval, p.s.This get the native sampling frequency of the sensor, the SDK will resample it and stream
                Command = TEXT("FRAME INTERVAL?");
                Response = SendCommand(Command);
                float Interval = FCString::Atof(*Response);
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Sampling frame interval: %.2f"), Interval);

                // EMG samples in a frame and its sampling rate
                Command = TEXT("MAX SAMPLES EMG?");
                Response = SendCommand(Command);
                float EMGSamples = FCString::Atof(*Response);
                EMGSensorNativeSampleInterval = Interval / EMGSamples;
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("EMG native sampling sampling interval: %.5f."), EMGSensorNativeSampleInterval);

                // EMG samples in a frame and its sampling rate
                Command = TEXT("MAX SAMPLES AUX?");
                Response = SendCommand(Command);
                float AUXSamples = FCString::Atof(*Response);
                AUXSensorNativeSampleInterval = Interval / AUXSamples;
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("AUX native sampling sampling interval: %.5f."), AUXSensorNativeSampleInterval);
                
                // Decide sampling rate
                if (ServerBackwardsComatability) // interval are locked according to server
                {
                    if (ServerUpSampling)
                        EMGSampleInterval = EMG_SERVER_SAMPLE_INTERVAL_BACK_UP;
                    else
                        EMGSampleInterval = EMG_SERVER_SAMPLE_INTERVAL_BACK_NUP;

                    AUXSampleInterval = AUX_SERVER_SAMPLE_INTERVAL_BACK;
                }
                else
                {
                    EMGSampleInterval = EMGSensorNativeSampleInterval;
                    AUXSampleInterval = AUXSensorNativeSampleInterval;
                }
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("EMG actual sampling rate: %.1f, sampling interval: %.5f."), 1.0f / EMGSampleInterval, EMGSampleInterval);
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("AUX actual sampling rate: %.1f, sampling interval: %.5f."), 1.0f / AUXSampleInterval, AUXSampleInterval);
                
            }
            else
            {
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("You may have forgot to connect sensors!"));
            }
            bInitialized = true;

            // Game thread
            Async(EAsyncExecution::TaskGraphMainThread, [this]()
                {
                    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys sensors are initialized."));
                });
        });

    
    return bConnected;
}

void UDelsysTrignoEMG::Close()
{
    // If not connected return
    if (!bConnected)
    {
        UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("No connection to Delsys Trigno Control Utility found!"));
        return;
    }

    // If acquiring data return
    if (bAcquiring || bRecording)
    {
        UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Cannot close while acquiring or recording data!"));
        return;
    }

    // Send QUIT command to server
    Async(EAsyncExecution::Thread, [this]()
        {
            FString Response = SendCommand(COMMAND_QUIT);

            bConnected = false;
            UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Disconnect with Delsys Trigno Control Utility."));
            // Game thread
            Async(EAsyncExecution::TaskGraphMainThread, [this]()
                {

                });
        });

}

void UDelsysTrignoEMG::StartAcquisition()
{
    if (!bConnected)
    {
        return;
        UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("No connection to Delsys Trigno Control Utility found!"));
    }
    

    // Get socket subsystem
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

    // Create socket for EMG and IMU
    EMGDataSocket = MakeShareable(SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DelsysEMDDataSocket"), false));
    EMGDataSocket->SetNonBlocking(false); // Blocking for reques-reply mode
    EMGDataSocket->SetReuseAddr(true);
    AUXDataSocket = MakeShareable(SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DelsysIMUDataSocket"), false));
    AUXDataSocket->SetNonBlocking(false); // Blocking for reques-reply mode
    AUXDataSocket->SetReuseAddr(true);

    // Set IP and port
    FIPv4Address EMGIP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), EMGIP);
    TSharedRef<FInternetAddr> EMGAddr = SocketSubsystem->CreateInternetAddr();
    EMGAddr->SetIp(EMGIP.Value);
    EMGAddr->SetPort(EMG_DATA_PORT);
    
    FIPv4Address AUXIP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), AUXIP);
    TSharedRef<FInternetAddr> AUXAddr = SocketSubsystem->CreateInternetAddr();
    AUXAddr->SetIp(AUXIP.Value);
    AUXAddr->SetPort(AUX_DATA_PORT);

    // Connect socket
    if (EMGDataSocket->Connect(*EMGAddr))
    {
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Connected to EMG data port."));
    }
    else
    {
        UE_LOG(LogDelsysTrignoEMG, Error, TEXT("Failed to connect to EMG data port!"));
    }

    if (AUXDataSocket->Connect(*AUXAddr))
    {
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Connected to AUX (IMU) data port."));
    }
    else
    {
        UE_LOG(LogDelsysTrignoEMG, Error, TEXT("Failed to connect to AUX (IMU) data port!"));
    }
    

    // Send start command to server to let it start streaming data
    // Use async task to avoid blocking the game thread
    Async(EAsyncExecution::Thread, [this]()
    {
        while (!bInitialized)
        {
            FPlatformProcess::Sleep(2.0f);// wait for a while for initialisation
        }

        // Start worker thread that acquire the data
        TrignoAcquisitionThreadInstance = MakeUnique<FTrignoAcquisitionThread>(this);
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG acquisition thread starts."));

        FString Response = SendCommand(COMMAND_START);

        if (Response.StartsWith("OK"))
        {
            bAcquiring = true;
            UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Server responds OK to start acquisition."));
        }
        else
        {
            bAcquiring = false;
            UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Server refuses to start acquisition!"));
        }

        // Game thread
        Async(EAsyncExecution::TaskGraphMainThread, [this]()
            {

            });
    });

}

void UDelsysTrignoEMG::StopAcquisition()
{
    if (!bAcquiring) return;

    bAcquiring = false;
    TrignoAcquisitionThreadInstance->Stop();
    
    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG sensor acquisition thread stop!"));

    // Add socket related stuff
    //Send stop command to server
    Async(EAsyncExecution::Thread, [this]()
        {
       
            FString Response = SendCommand(COMMAND_STOP);
            if (!Response.StartsWith("O"))
            {
                UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Server refuses to stop acquisition, further actions may fail!"));
            }

            // Game thread
            Async(EAsyncExecution::TaskGraphMainThread, [this]()
                {
                    
                });
        });

}

void UDelsysTrignoEMG::StartRecording(const FString& FilePath)
{
    if (!bAcquiring) return;
    
    EMGFileManager = new FTextFileManager(FString::Printf(TEXT("%s_EMG"), *FilePath));
    AUXFileManager = new FTextFileManager(FString::Printf(TEXT("%s_AUX"), *FilePath));

    // Save EMG header
    FString EMGHeaderString;
    EMGHeaderString.Append("System_Time,Time,");
    for (int i = 0; i < ActiveSensorList.SensorChannels.Num(); i++)
    {
        EMGHeaderString.Append(FString::Printf(TEXT("EMG_Sensor%d"), ActiveSensorList.SensorChannels[i]+1));
        // ActiveSensorChannel records the array index so +1

        if (i != ActiveSensorList.SensorChannels.Num()-1)
        {
            EMGHeaderString.Append(","); // Comma for csv format, but not for the last value
        }
        else
        {
            EMGHeaderString.Append(LINE_TERMINATOR);
        }
    }
    EMGFileManager->NewContent(EMGHeaderString);

    // Save AUX header
    FString AUXHeaderString;
    AUXHeaderString.Append("System_Time,Time,");
    for (int i = 0; i < ActiveSensorList.SensorChannels.Num(); i++)
    {
        int SensorChannel = ActiveSensorList.SensorChannels[i] + 1;
        if (ActiveSensorList.AUXDataChannels[i] >= 3)
        {
            AUXHeaderString.Append(FString::Printf(TEXT("ACC_X_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("ACC_Y_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("ACC_Z_Sensor%d"), SensorChannel));
        }
        
        if (ActiveSensorList.AUXDataChannels[i] >= 6)
        {
            AUXHeaderString.Append(FString::Printf(TEXT(",GYRO_X_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("GYRO_Y_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("GYRO_Z_Sensor%d"), SensorChannel));
        }

        if (ActiveSensorList.AUXDataChannels[i] == 9)
        {
            AUXHeaderString.Append(FString::Printf(TEXT(",ORIEN_X_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("ORIEN_Y_Sensor%d,"), SensorChannel));
            AUXHeaderString.Append(FString::Printf(TEXT("ORIEN_Z_Sensor%d"), SensorChannel));
        }


        if (i != ActiveSensorList.SensorChannels.Num() - 1)
        {
            AUXHeaderString.Append(","); // Comma for csv format, but not for the last value
        }
        else
        {
            AUXHeaderString.Append(LINE_TERMINATOR);
        }
    }
    AUXFileManager->NewContent(AUXHeaderString);



    EMGTimeCount = 0.0f;
    AUXTimeCount = 0.0f;
    bRecording = true;

    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG data recording start."));
    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("EMG data will be saved to: %s"), *FilePath);
   
  
}

void UDelsysTrignoEMG::StopRecording()
{
    if (!bRecording) return;

    bRecording = false;
    EMGFileManager->Stop();
    AUXFileManager->Stop();
    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG data recording stop."));
}

void UDelsysTrignoEMG::AcquireData()
{
    EMGTimeCount += EMGSampleInterval;
    AUXTimeCount += AUXSampleInterval;

    FString EMGDataString;
    FString AUXDataString;

    // Time Stamp
    FDateTime Now = FDateTime::Now();
    int32 Year = Now.GetYear();
    int32 Month = Now.GetMonth();
    int32 Day = Now.GetDay();
    int32 Hour = Now.GetHour();
    int32 Minute = Now.GetMinute();
    int32 Second = Now.GetSecond();
    int32 Millisecond = Now.GetMillisecond();
    FString TimeString = FString::Printf(TEXT("%04d-%02d-%02d %02d:%02d:%02d.%03d,"),
        Year,Month, Day, Hour, Minute, Second, Millisecond);

    EMGDataString.Append(TimeString);
    EMGDataString.Append(FString::Printf(TEXT("%.5f,"), EMGTimeCount));

    AUXDataString.Append(TimeString);
    AUXDataString.Append(FString::Printf(TEXT("%.5f,"), AUXTimeCount));

    

    // Acquire EMG data
    TempEMGDataList.Init(0.0f, LEN_EMG_BUFFER);// Max 16 EMG data for each sample
    for (int i = 0; i < MAX_SENSOR; i++) // 16 sensors loop
    {
        uint8 TempBuffer[4]; // buffer for response
        int32 BytesRead = 0;
        
        if (EMGDataSocket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead)  && BytesRead)
        {
            float EMGValue;
            FMemory::Memcpy(&EMGValue, TempBuffer, 4); // Convert the 4 bytes data to a float
            TempEMGDataList[i] = EMGValue;
            
            // Save EMG data
            if (ActiveSensorList.SensorChannels.Contains(i))
            {
                EMGDataString.Append(FString::Printf(TEXT("%.7e"), TempEMGDataList[i])); // 7 digits float precision

                if (i != ActiveSensorList.SensorChannels.Last())
                {
                    EMGDataString.Append(","); // Comma for csv format, but not for the last value
                }
                else
                {
                    EMGDataString.Append(LINE_TERMINATOR);
                }
            }
        }
    }

    // Acquire AUX data
    TempAUXDataList.Init(0.0f, LEN_AUX_BUFFER); // Max 16*9 AUX data for each sample
    for (int i = 0; i < MAX_SENSOR; i++)
    {
        for (int j = 0; j < LEN_AUX_BUFFER / MAX_SENSOR; j++) // each sensor has 9 data channels in
        {
            uint8 TempBuffer[4]; // buffer for response
            int32 BytesRead = 0;

            if (AUXDataSocket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead) && BytesRead)
            {
                float AUXValue;
                FMemory::Memcpy(&AUXValue, TempBuffer, 4); // Convert the 4 bytes data to a float
                int Index = i * 16 + j;
                TempAUXDataList[Index] = AUXValue;

                // Save EMG data
                if (ActiveSensorList.SensorChannels.Contains(i) && j < ActiveSensorList.AUXDataChannels[i])
                {
                    AUXDataString.Append(FString::Printf(TEXT("%.7e"), TempAUXDataList[Index])); // 7 digits float precision

                    if (i != ActiveSensorList.SensorChannels.Last())
                    {
                        AUXDataString.Append(","); // Comma for csv format, but not for the last value
                    }
                    else
                    {
                        AUXDataString.Append(LINE_TERMINATOR);
                    }
                }
            }
        }
    }

    if (bRecording)
    {
        EMGFileManager->NewContent(EMGDataString);
        AUXFileManager->NewContent(AUXDataString);
    }
        


}

TArray<float> UDelsysTrignoEMG::GetRawEMGData() const
{
    return TArray<float>();
}

FString UDelsysTrignoEMG::SendCommand(FString Command)
{
    
        FString Response = "";

        if (bConnected)
        {
            UE_LOG(LogDelsysTrignoEMG, Log, TEXT("-> %s"), *Command);

            // Convert the command to a byte array 
            FString CommandString = Command;
            CommandString.Append("\r\n");
            CommandString.Append("\r\n");// require two line terminator for a valid command

            TCHAR* CommandChar = CommandString.GetCharArray().GetData(); //Get FString char array

            FTCHARToUTF8 Converter(CommandChar); //Convert char to UTF8 code

            // Send the command
            int32 BytesSent = 0;
            bool bSent = false;
            while (true)
            {
                if (CommandSocket->Send((uint8*)Converter.Get(), Converter.Length(), BytesSent))
                {
                    if (BytesSent == Converter.Length())
                    {
                        bSent = true;
                        break;
                    }
                }
                else
                {
                    break; // Error in sending
                }
            }

            
            // Receive the reply
            if (bSent)
            {
                // Read the response 
                bool bReceived = false;
                TArray<uint8> ResponseBytes;
                uint8 TempBuffer[1]; // Buffer for response
                int32 BytesRead = 0;
                int8 NewlineCount = 0;
                while (!bReceived)
                {

                    if (CommandSocket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead))
                    {
                        if (BytesRead <= 0)
                        {
                            break; // No data read
                        }

                        // Stop at newline
                        if (TempBuffer[0] == '\n')
                        {
                            NewlineCount += 1;
                            if (NewlineCount == 2)// Two consecutive new line = end of the response
                            {
                                bReceived = true;
                                break;
                            }    
                        }

                        // Append to data
                        ResponseBytes.Add(TempBuffer[0]);
                    }
                    else
                    {
                        break; // Socket error or disconnected
                    }
                }

                if (bReceived)
                {
                    FString ResponseString = FString(UTF8_TO_TCHAR((const char*)ResponseBytes.GetData()));
                    Response = ResponseString;
                    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Get response from Trigno Control Utility!"));
                    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("<-  %s"), *Response);
                }
            }
            else
            {
                UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Failed to send command to Trigno Control Utility!"));
            }
        }
        else
        {
            UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Delsys Trigno Control Utility not connected!"));
        }

        return Response;
 
}
