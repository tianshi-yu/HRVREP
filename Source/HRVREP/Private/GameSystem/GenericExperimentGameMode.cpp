// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/GenericExperimentGameMode.h"

//
// Override parent
//
void AGenericExperimentGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ExperimentState = EExperimentState::InitExperiment;
	InitSystem();
	InitExperiment();
	
	ExperimentState = EExperimentState::Training;
	InitTraining();
	
	
}

//
// Override parent
//
void AGenericExperimentGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	switch(ExperimentState)
	{
		case EExperimentState::InitExperiment:
			break;

		case EExperimentState::Training:
			bool TrainingComplete;
			HandleTrainingProcess();
			CheckTrainingComplete(TrainingComplete);
			if (TrainingComplete)
			{
				HandleTrainingComplete();
				ExperimentState = EExperimentState::PerformingTask;
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: Training Complete. Start Task."));
			}	
			break;

		case EExperimentState::PerformingTask:
			HandleTaskProcess();
			bool TaskComplete;
			CheckTaskComplete(TaskComplete);
			if (TaskComplete)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: Task Complete."));
				HandleTaskComplete();
				bool IterationComplete;
				CheckIterationComplete(IterationComplete);
				if (IterationComplete)
				{
					UE_LOG(LogTemp, Warning, TEXT("Game Mode: Iteration Complete."));
					HandleIterationComplete();
					bool SessionComplete;
					CheckSessionComplete(SessionComplete);
					if (SessionComplete)
					{
						UE_LOG(LogTemp, Warning, TEXT("Game Mode: Session Complete."));
						HandleSessionComplete();
						bool ExperimentComplete;
						CheckExperimentComplete(ExperimentComplete);
						if (ExperimentComplete)
						{
							UE_LOG(LogTemp, Warning, TEXT("Game Mode: Experiment Complete."));
							EndPlay(EEndPlayReason::Quit);
						}
					}
				}
			}
			break;
	}
	
}

//
// Override parent
//
void AGenericExperimentGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGenericExperimentGameMode::InitSystem_Implementation()
{

}

void AGenericExperimentGameMode::InitTraining_Implementation()
{
	
}

void AGenericExperimentGameMode::InitExperiment_Implementation()
{
	TaskListNumber = 0;
	IterationNumber = 1;
	SessionNumber = 1;
}

void AGenericExperimentGameMode::InitSession_Implementation()
{

}

void AGenericExperimentGameMode::InitIteration_Implementation()
{

}



void AGenericExperimentGameMode::HandleTrainingProcess_Implementation()
{
}

void AGenericExperimentGameMode::CheckTrainingComplete_Implementation(bool& OutComplete)
{
}

void AGenericExperimentGameMode::HandleTrainingComplete_Implementation()
{
}

void AGenericExperimentGameMode::HandleTaskProcess_Implementation()
{
}

void AGenericExperimentGameMode::CheckTaskComplete_Implementation(bool& OutComplete)
{
}

void AGenericExperimentGameMode::HandleTaskComplete_Implementation()
{
	TaskListNumber += 1;
}

void AGenericExperimentGameMode::CheckIterationComplete_Implementation(bool& OutComplete)
{
	if (TaskListNumber >= TaskList.Num())
	{
		OutComplete = true;
		TaskListNumber = 0;
		IterationNumber += 1;
	}
	else
	{
		OutComplete = false;
	}
}

void AGenericExperimentGameMode::HandleIterationComplete_Implementation()
{
	IterationNumber += 1;
}

void AGenericExperimentGameMode::CheckSessionComplete_Implementation(bool& OutComplete)
{
	if (IterationNumber > MaxIteration)
	{
		OutComplete = true;
		IterationNumber = 1;
		SessionNumber += 1;
	}
	else
	{
		OutComplete = false;
	}
}

void AGenericExperimentGameMode::HandleSessionComplete_Implementation()
{
}

void AGenericExperimentGameMode::CheckExperimentComplete_Implementation(bool& OutComplete)
{
	if (SessionNumber > MaxSession)
	{
		OutComplete = true;
	}
	else
	{
		OutComplete = false;
	}
}

