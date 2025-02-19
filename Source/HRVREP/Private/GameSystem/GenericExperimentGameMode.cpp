// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/GenericExperimentGameMode.h"

//
// Override parent
//
void AGenericExperimentGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ExperimentState = EExperimentState::InitExperiment;
	InitExperiment();
	InitSystem();
	
	
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
			HandleTrainingProcess();
			bool TrainingTaskFail;
			CheckTaskFail(TrainingTaskFail);
			bool TrainingComplete;
			CheckTrainingComplete(TrainingComplete);
			if (TrainingTaskFail)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: Task Failed."));
				InitTask();
				break;
			}
			else if (TrainingComplete)
			{
				HandleTrainingComplete();
				ExperimentState = EExperimentState::PerformingTask;
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: Training Complete. Start Task."));
				break;
			}	
			break;

		case EExperimentState::PerformingTask:
			HandleTaskProcess();
			bool TaskFail;
			CheckTaskFail(TaskFail);
			bool TaskComplete;
			CheckTaskComplete(TaskComplete);
			if (TaskFail)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode: Task Failed."));
				ExperimentState = EExperimentState::TaskFail;
				break;
			}
			else if (TaskComplete)
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
							ExperimentState = EExperimentState::EndExperiment;
							EndPlay(EEndPlayReason::Quit);
							break; // no need to go through iteration and session init
						}
						InitSession();
					}
					InitIteration();
				}
				
			}
			break;

			case EExperimentState::TaskFail:
				HandleTaskFail();
				InitTask();
				ExperimentState = EExperimentState::PerformingTask;
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
	TaskNumber = 0;
	TaskAttemptNumber = 1;
	IterationNumber = 1;
	SessionNumber = 1;
}

void AGenericExperimentGameMode::InitSession_Implementation()
{

}

void AGenericExperimentGameMode::InitIteration_Implementation()
{

}

void AGenericExperimentGameMode::InitTask_Implementation()
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
	TaskAttemptNumber = 1;
	TaskNumber += 1;
}

void AGenericExperimentGameMode::CheckTaskFail_Implementation(bool& OutFail)
{

}

void AGenericExperimentGameMode::HandleTaskFail_Implementation()
{
	TaskAttemptNumber += 1;
}

void AGenericExperimentGameMode::CheckIterationComplete_Implementation(bool& OutComplete)
{
	if (TaskNumber >= TaskList.Num())
	{
		OutComplete = true;
	}
	else
	{
		OutComplete = false;
	}
}

void AGenericExperimentGameMode::HandleIterationComplete_Implementation()
{
	TaskNumber = 0;
	IterationNumber += 1;
}

void AGenericExperimentGameMode::CheckSessionComplete_Implementation(bool& OutComplete)
{
	if (IterationNumber > MaxIteration[SessionNumber - 1])
	{
		OutComplete = true;
	}
	else
	{
		OutComplete = false;
	}
}

void AGenericExperimentGameMode::HandleSessionComplete_Implementation()
{
	IterationNumber = 1;
	SessionNumber += 1;
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

