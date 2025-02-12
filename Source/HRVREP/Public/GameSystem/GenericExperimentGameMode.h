// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericExperimentGameMode.generated.h"

/**
 * Generic experiment game mode class
 */

UENUM(BlueprintType)
enum class EExperimentState : uint8
{
	InitExperiment      UMETA(DisplayName = "InitExperiment"),
	Training   UMETA(DisplayName = "Training"),
	PerformingTask   UMETA(DisplayName = "PerformingTask"),
	EndExperiment UMETA(DisplayName = "EndExperiment")
};

UENUM(BlueprintType)
enum class ETaskState : uint8
{
	PrepareTask UMETA(DisplayName = "PrepareTask"),
	PrepareTaskComplete UMETA(DisplayName = "PrepareTaskComplete"),
	PrepareTaskFail UMETA(DisplayName = "PrepareTaskFail"),
	InTask   UMETA(DisplayName = "InTask"),
	TaskFail   UMETA(DisplayName = "TaskFail"),
	TaskComplete   UMETA(DisplayName = "TaskComplete")
};




UCLASS(Blueprintable)
class HRVREP_API AGenericExperimentGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Override parent 
	virtual void BeginPlay() override;

	// Override parent
	virtual void Tick(float DeltaSecond) override;

	// Override parent
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// 
	//******************************* Initialisation Part **************************************//
	//
	
	// Initialise the experimental system, such as user, sensor, objects...
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Init System"))
	void InitSystem();
	virtual void InitSystem_Implementation();// If derive override this function

	// Initialise the training module
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Init Training"))
	void InitTraining();
	virtual void InitTraining_Implementation();

	// Initialise the experiment module
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Init Experiment"))
	void InitExperiment();
	virtual void InitExperiment_Implementation();
	
	// Initialise session settings
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Init Session"))
	void InitSession();
	virtual void InitSession_Implementation();

	// Initialise iteration
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Init Iteration"))
	void InitIteration();
	virtual void InitIteration_Implementation();


	// 
	//******************************* Handle Behaviour Part **************************************//
	//
	// Handle traning process
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Training"))
	void HandleTrainingProcess();
	virtual void HandleTrainingProcess_Implementation();

	// Check if traning is done
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Check Training Complete"))
	void CheckTrainingComplete(bool& OutComplete);
	virtual void CheckTrainingComplete_Implementation(bool& OutComplete);

	// Handle traning process completion
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Training Complete"))
	void HandleTrainingComplete();
	virtual void HandleTrainingComplete_Implementation();

	// Handle task process
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Task Process"))
	void HandleTaskProcess();
	virtual void HandleTaskProcess_Implementation();
	
	// Check if task is done
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Check Task Complete"))
	void CheckTaskComplete(bool& OutComplete);
	virtual void CheckTaskComplete_Implementation(bool& OutComplete);

	// Handle task completion
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Task Complete"))
	void HandleTaskComplete();
	virtual void HandleTaskComplete_Implementation();

	// Check if iteration is done
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Check Iteration Complete"))
	void CheckIterationComplete(bool& OutComplete);
	virtual void CheckIterationComplete_Implementation(bool& OutComplete);

	// Handle iteration completion
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Iteration Complete"))
	void HandleIterationComplete();
	virtual void HandleIterationComplete_Implementation();

	// Check if session is done
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Check Session Complete"))
	void CheckSessionComplete(bool& OutComplete);
	virtual void CheckSessionComplete_Implementation(bool& OutComplete);

	// Handle session completion
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Handle Session Complete"))
	void HandleSessionComplete();
	virtual void HandleSessionComplete_Implementation();

	// Check if experiment is done
	UFUNCTION(BlueprintNativeEvent, Category = "Experiment Game mode", meta = (Keywords = "Check Experiment Complete"))
	void CheckExperimentComplete(bool& OutComplete);
	virtual void CheckExperimentComplete_Implementation(bool& OutComplete);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experiment")
	EExperimentState ExperimentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experiment")
	ETaskState TaskState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experiment")
	TArray<FString> TaskList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experiment")
	int32 TaskListNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experiment")
	int32 IterationNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experiment")
	TArray<int32> MaxIteration;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experiment")
	int32 SessionNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experiment")
	int32 MaxSession;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experiment")
	bool Debug;


};
