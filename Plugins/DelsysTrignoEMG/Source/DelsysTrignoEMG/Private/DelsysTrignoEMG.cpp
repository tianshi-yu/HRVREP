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

    // Build a list of connected sensor types
    for (int i = 1; i <= 16; i++)
    {
        FString Query = FString::Printf(TEXT("SENSOR %d %s"), i, *COMMAND_SENSOR_TYPE);
        FString Response = SendCommand(Query);

        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("-> %s"), *Query);
        UE_LOG(LogDelsysTrignoEMG, Log, TEXT("<-  %s"), *Response);

        SensorTypeList.Add(Response.Contains("INVALID") ? SensorTypes::NoSensor : SensorTypeDict[Response]);
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
    SendCommand("UPSAMPLE ON");


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

    // Send QUIT command to server
    SendCommand(COMMAND_QUIT);
    bConnected = false;
    CommandSocket->Close();
    UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Disconnect with Delsys Trigno Control Utility!"));
}

void UDelsysTrignoEMG::StartAcquisition()
{
    if (!bConnected) return;

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
    TrignoWorkerThreadInstance = MakeUnique<FTrignoWorkerThread>(this);
    UE_LOG(LogDelsysTrignoEMG, Log, TEXT("Delsys Trigno EMG acquisition started!"));

    // Send start command to server to let it start streaming data
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
}

void UDelsysTrignoEMG::StopAcquisition()
{
    if (!bConnected || !bAcquiring) return;

    // Add socket related stuff
    //Send stop command to server

    bAcquiring = false;
    TrignoWorkerThreadInstance->Stop();

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

FString UDelsysTrignoEMG::SendCommand(const FString& Command)
{
    FString Response = "";

    if (bConnected)
    {
        if (CommandSocket)
        {
            // Convert the command to a byte array 
            TArray<uint8> CommandBytes;
            FString CommandString = Command + TEXT("\n");
            FTCHARToUTF8 Converter(*CommandString);
            CommandBytes.Append((const uint8*)Converter.Get(), Converter.Length());

            // Send the command
            int32 BytesSent = 0;
            bool bSent = CommandSocket->Send(CommandBytes.GetData(), CommandBytes.Num(), BytesSent);
            if (bSent)
            {
                // Read the response (this assumes you can handle socket reading correctly)
                TArray<uint8> ResponseBytes;
                uint8 TempBuffer[1024]; // buffer for response

                int32 BytesRead = 0;
                bool bReceived = CommandSocket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead);
                if (bReceived && BytesRead > 0)
                {
                    ResponseBytes.Append(TempBuffer, BytesRead);
                    FString ResponseString = FString(UTF8_TO_TCHAR((const char*)ResponseBytes.GetData()));
                    Response = ResponseString;
                }
            }
            else
            {
                UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Failed to send command to Trigno sensor base!"));
            }
        }
    }
    else
    {
        UE_LOG(LogDelsysTrignoEMG, Warning, TEXT("Delsys Trigno Control Utility not connected!"));
    }

    return Response;
}
