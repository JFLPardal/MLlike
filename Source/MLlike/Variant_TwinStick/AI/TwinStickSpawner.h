// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwinStickNPC.h"
#include "TwinStickSpawner.generated.h"

class ARecastNavMesh;
class UEnemyDefinitionDataAsset;

/**
 *  A simple NPC spawner for a Twin Stick Shooter game
 */
UCLASS(abstract)
class ATwinStickSpawner : public AActor
{
	GENERATED_BODY()

protected:
	/** Type of NPC to spawn */
	UPROPERTY(EditAnywhere, Category="NPC Spawner")
	TSubclassOf<ATwinStickNPC> NPCClass;

	TArray<TSoftObjectPtr<UEnemyDefinitionDataAsset>> EnemyDefinitions;

	/** Radius around the spawner where it can spawn NPCs */
	UPROPERTY(EditAnywhere, Category="NPC Spawner", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float SpawnRadius = 100.0f;

	/** Pointer to the recast nav mesh actor, used to provide NPC spawn locations */
	TObjectPtr<ARecastNavMesh> NavData;

public:
	ATwinStickSpawner();

	void SpawnNPC();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeEnemyDefinitions();
};
