// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RunDirectorSubsystem.generated.h"

DECLARE_DELEGATE(FOnSpawnNextWave);

class ATwinStickGameMode;

/**
 * 
 */
UCLASS()
class MLLIKE_API URunDirectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterGameMode(ATwinStickGameMode* const GameMode);
	void UnregisterGameMode(ATwinStickGameMode* const GameMode);

	FOnSpawnNextWave OnSpawnNextWave;

private:
	void HandleWaveCleared();

	FDelegateHandle OnWaveClearedHandle;
};
