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
    Close();
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
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Connected to Delsys Trigno Control Utility server!"));
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
                for (int i = 1; i <= 16; i++)
                {
                    Command = FString::Printf(TEXT("SENSOR %d %s"), i, *COMMAND_SENSOR_TYPE);
                    Response;
                    Response = SendCommand(Command);

                    //SensorTypeList.Add(Response.Contains("INVALID") ? SensorTypes::NoSensor : SensorTypeDict[Response]);
                    SensorTypeList.Add(Response.Contains("INVALID") ? SensorTypes::NoSensor : SensorTypes::SensorTrignoImu);
                }

                // Record the active sensor channel number
                for (int i = 0; i < 16; i++)
                {
                    if (SensorTypeList[i] == SensorTypes::SensorTrignoImu)
                    {
                        ActiveSensorChannels.Add(i + 1);
                    }
                }

                // Config sensors, not finished to be added
                Command = TEXT("UPSAMPLE ON");
                Response = SendCommand(Command);

                Response = SendCommand(COMMAND_START);

            }

            // Game thread
            Async(EAsyncExecution::TaskGraphMainThread, [this]()
                {
                    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Sensor Configured!"));
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
    if (bAcquiring)
    {
        UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Cannot close while acquiring data!"));
        return;
    }

    // Send QUIT command to server, no need for thread as the gaming is ending
    FString Response = SendCommand(COMMAND_QUIT);

    bConnected = false;
    UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Disconnect with Delsys Trigno Control Utility!"));
}

void UDelsysTrignoEMG::StartAcquisition()
{
    if (!bConnected) return;
    UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("No connection to Delsys Trigno Control Utility found!"));

    // Get socket subsystem
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

    // Create socket for EMG and IMU
    EMGDataSocket = MakeShareable(SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DelsysEMDDataSocket"), false));
    IMUDataSocket = MakeShareable(SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DelsysEMDDataSocket"), false));

    // Set IP and port
    FIPv4Address EMGIP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), EMGIP);
    TSharedRef<FInternetAddr> EMGAddr = SocketSubsystem->CreateInternetAddr();
    EMGAddr->SetIp(EMGIP.Value);
    EMGAddr->SetPort(EMG_DATA_PORT);
    
    FIPv4Address IMUIP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), IMUIP);
    TSharedRef<FInternetAddr> IMUAddr = SocketSubsystem->CreateInternetAddr();
    IMUAddr->SetIp(IMUIP.Value);
    IMUAddr->SetPort(IMU_DATA_PORT);


    // Start worker thread that acquire the data
    bAcquiring = true;
    TrignoAcquisitionThreadInstance = MakeUnique<FTrignoAcquisitionThread>(this);
    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG acquisition started!"));

    // Send start command to server to let it start streaming data
    // Use async task to avoid blocking the game thread
    Async(EAsyncExecution::Thread, [this]()
    {
        //FString Response = SendCommand(COMMAND_START);
        FString Response = SendCommand(COMMAND_START);


        if (Response.StartsWith("OK"))
        {
            UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Server responds OK to start acquisition!"));
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
    if (!bConnected || !bAcquiring) return;

    // Add socket related stuff
    //Send stop command to server

    bAcquiring = false;
    TrignoAcquisitionThreadInstance->Stop();

    UE_LOG(LogTemp, Log, TEXT("Delsys Trigno EMG acquisition stopped!"));
}

void UDelsysTrignoEMG::StartRecording(const FString& FilePath)
{

    
}

void UDelsysTrignoEMG::StopRecording()
{

}

void UDelsysTrignoEMG::AcquireData()
{
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
            CommandString.Append(LINE_TERMINATOR);
            CommandString.Append(LINE_TERMINATOR);// require two line terminator for a valid command

            TCHAR* CommandChar = CommandString.GetCharArray().GetData();

            FTCHARToUTF8 Converter(CommandChar);

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
                    break; // error in sending
                }
            }

            
            // Receive the reply
            if (bSent)
            {
                // Read the response 
                bool bReceived = false;
                TArray<uint8> ResponseBytes;
                uint8 TempBuffer[1]; // buffer for response
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
                            if (NewlineCount == 2)// two consecutive new line = end of the response
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
