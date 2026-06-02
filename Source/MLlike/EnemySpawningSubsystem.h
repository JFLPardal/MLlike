// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemySpawningSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWaveCleared);

class ATwinStickSpawner;

/**
 * 
 */
UCLASS()
class MLLIKE_API UEnemySpawningSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
		
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void EnemyDestroyed();

	FOnWaveCleared OnWaveCleared;

#if !UE_BUILD_SHIPPING
	void DebugClearWave();
#endif

protected:
	// needed to make sure this subsystem is not initialized during editor creation, when RunDirectorSubsystem might not be available
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	void HandleOnSpawnNextWave();

private:
	int32 NumRemaningEnemiesInWave = 0;
	int32 NumMaxEnemiesPerWave = 3;
	TArray<ATwinStickSpawner*> AllSpawners;
	TArray<ATwinStickSpawner*> UnusedSpawners;
};
