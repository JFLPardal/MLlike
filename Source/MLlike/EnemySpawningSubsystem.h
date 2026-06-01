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

protected:
	// needed to make sure this subsystem is not initialized during editor creation, when RunDirectorSubsystem might not be available
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	void HandleOnSpawnNextWave();

	int32 NumActiveEnemies = 0;
	int32 NumMaxActiveEnemies = 3;
	TArray<ATwinStickSpawner*> Spawners;
};
