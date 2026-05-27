// Fill out your copyright notice in the Description page of Project Settings.


#include "RunDirectorSubsystem.h"

#include "TwinStickGameMode.h"
#include "MLlikeLogCategories.h"

void URunDirectorSubsystem::RegisterGameMode(ATwinStickGameMode* const GameMode)
{
	if (!IsValid(GameMode))
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - GameMode is not valid - won't be able to be informed when the wave is cleared which will affect perk screen / spawning new waves"), TEXT(__FUNCSIG__));
		return;
	}

	OnWaveClearedHandle = GameMode->OnWaveCleared.AddUObject(this, &URunDirectorSubsystem::HandleWaveCleared);
}

void URunDirectorSubsystem::HandleWaveCleared()
{
	OnSpawnNextWave.ExecuteIfBound();
}

void URunDirectorSubsystem::UnregisterGameMode(ATwinStickGameMode* const GameMode)
{
	if (!IsValid(GameMode))
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - GameMode is not valid - won't be able to unregister any events that were previously registered"), TEXT(__FUNCSIG__));
		return;
	}

	GameMode->OnWaveCleared.Remove(OnWaveClearedHandle);
}