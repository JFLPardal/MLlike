// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RunDirectorSubsystem.generated.h"

DECLARE_DELEGATE(FOnSpawnNextWave);

class FAssetRegistryModule;
class UChoiceOptionConfig;
class UChoiceScreenConfig;
class UEnemySpawningSubsystem;
class UWorld;
class URarityWeightConfig;

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

	void FindPerkSelectionScreenConfig(const FAssetRegistryModule& AssetRegistryModule);
	void FindPerkRarityWeightConfig(const FAssetRegistryModule& AssetRegistryModule);

private:
	FDelegateHandle OnWaveClearedHandle;

	UPROPERTY()
	TArray<TSoftObjectPtr<UChoiceScreenConfig>> PerkSelectionScreenConfig;

	UPROPERTY()
	TArray<TObjectPtr<UChoiceOptionConfig>> UnchosenPerks;

	bool bInitializedUnchosenPerks = false;
	
	UPROPERTY()
	TSoftObjectPtr<URarityWeightConfig> PerkRarityWeights;
};
