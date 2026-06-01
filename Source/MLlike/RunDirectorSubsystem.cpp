// Fill out your copyright notice in the Description page of Project Settings.


#include "RunDirectorSubsystem.h"

#include "Engine/World.h"
#include "ChoiceScreenWidget.h"
#include "MLlikeLogCategories.h"
#include "UISubsystem.h"
#include "EnemySpawningSubsystem.h"

void URunDirectorSubsystem::HandleWaveCleared()
{
	if (UUISubsystem* const UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(UISubsystem))
	{
		if (UChoiceScreenWidget* ChoiceScreen = UISubsystem->ShowPerkSelectionScreen(); IsValid(ChoiceScreen))
		{
			ChoiceScreen->OnChoiceMade.BindLambda([this]() { OnSpawnNextWave.ExecuteIfBound(); });
		}
	}
}


void URunDirectorSubsystem::RegisterEnemySpawningSubsystem(UEnemySpawningSubsystem* const EnemySpawningSubsystem)
{
	if (!IsValid(EnemySpawningSubsystem))
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - EnemySpawningSubsystem is not valid - won't be able to be informed when the wave is cleared which will affect perk screen / spawning new waves"), TEXT(__FUNCSIG__));
		return;
	}

	OnWaveClearedHandle = EnemySpawningSubsystem->OnWaveCleared.AddUObject(this, &URunDirectorSubsystem::HandleWaveCleared);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {OnSpawnNextWave.ExecuteIfBound(); });
	
}

void URunDirectorSubsystem::UnregisterEnemySpawningSubsystem(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (UEnemySpawningSubsystem* const EnemySpawningSubsystem = GetWorld()->GetSubsystem<UEnemySpawningSubsystem>(); IsValid(EnemySpawningSubsystem))
	{
		EnemySpawningSubsystem->OnWaveCleared.Remove(OnWaveClearedHandle);
	}
	else
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - EnemySpawningSubsystem is not valid - won't be able to unregister any events that were previously registered"), TEXT(__FUNCSIG__));
		return;
	}
}
