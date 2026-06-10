// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RunDirectorSubsystem.generated.h"

DECLARE_DELEGATE(FOnSpawnNextWave);

class UChoiceOptionConfig;
class UChoiceScreenConfig;
class UEnemySpawningSubsystem;
class UWorld;

/**
 * 
 */
UCLASS()
class MLLIKE_API URunDirectorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void RegisterEnemySpawningSubsystem(UEnemySpawningSubsystem* const EnemySpawningSubsystem);
	// needed? unused right now
	void UnregisterEnemySpawningSubsystem(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	FOnSpawnNextWave OnSpawnNextWave;

private:
	void HandleWaveCleared();
	void HandleChoiceMade(const UChoiceOptionConfig* const ChosenConfig);

	void FindPerkSelectionScreenConfig();

private:
	FDelegateHandle OnWaveClearedHandle;

	TArray<TSoftObjectPtr<UChoiceScreenConfig>> PerkSelectionScreenConfig;
	TArray<TObjectPtr<UChoiceOptionConfig>> UnchosenPerks;
	bool bInitializedUnchosenPerks = false;
};
