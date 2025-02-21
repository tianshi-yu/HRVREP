// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadarSensorActor.generated.h"

USTRUCT(BlueprintType)
struct FRadarData
{
	GENERATED_BODY()

public:
	FRadarData()
		: Distance(0.0f)
		, Bearing(0.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	double Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	double Bearing;

};


USTRUCT(BlueprintType)
struct FRadarConfig
{
	GENERATED_BODY()

public:
	FRadarConfig()
		: FOV(120.0f)
		, TraceLength(10000.0f)
		, TraceNumber(10)
		, TraceBearings()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	float FOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	float TraceLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	int TraceNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadarSensorActor")
	TArray<float> TraceBearings;
};


UCLASS()
class HRVREP_API ARadarSensorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadarSensorActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Config the sensor actor
	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Configure"))
	virtual void Configure(const FRadarConfig& InRadarConfig);

	// Getter 
	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Get Data"))
	const FRadarData& GetData() const;

	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Get Config"))
	const FRadarConfig& GetConfig() const;

	/*
	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Get Name"))
	const FString& GetName() const;

	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Set Name"))
	void SetName(const FString& InName);
	*/
	// Calculate the trace angles
	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Trace Angles"))
	TArray<float> CalculateTraceBearings(const FRadarConfig& InRadarConfig);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Setter
	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Set Data"))
	void SetData(const FRadarData& InRadarData);

	UFUNCTION(BlueprintCallable, Category = "RadarSensorActor", meta = (Keywords = "Set Config"))
	void SetConfig(const FRadarConfig& InRadarConfig);

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetData, BlueprintSetter = SetData, Category = "RadarSensorActor")
	FRadarData RadarData;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetConfig, BlueprintSetter = SetConfig, Category = "RadarSensorActor")
	FRadarConfig RadarConfig;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetName, BlueprintSetter = SetName, Category = "RadarSensorActor")
	FString RadarName;
	*/
};
