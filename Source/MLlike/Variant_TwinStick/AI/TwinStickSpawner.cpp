// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickSpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TwinStickNPC.h"
#include "TwinStickGameMode.h"

static TAutoConsoleVariable<bool> CVarEnableEnemySpawn(TEXT("ml.EnableEnemySpawn"), true, TEXT(""));

ATwinStickSpawner::ATwinStickSpawner()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ATwinStickSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// find the recast navmesh actor on the level
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecastNavMesh::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		NavData = Cast<ARecastNavMesh>(ActorList[0]);
	} else {

		UE_LOG(LogTemp, Log, TEXT("Could not find recast navmesh"));

	}

	// set up the spawn timer
	GetWorld()->GetTimerManager().SetTimer(SpawnGroupTimer, this, &ATwinStickSpawner::SpawnNPCGroup, SpawnGroupDelay, true);

	// spawn the first group of NPCs
	SpawnNPCGroup();
}

void ATwinStickSpawner::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the spawn timers
	GetWorld()->GetTimerManager().ClearTimer(SpawnGroupTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimer);
}

void ATwinStickSpawner::SpawnNPCGroup()
{
	// doing this here instead of when the timer is set means we don't need to restart the game to enable the command
	if (CVarEnableEnemySpawn.GetValueOnGameThread())
	{
		// reset the group spawn counter
		SpawnCount = 0;

		// check if we're still under the max NPC cap
		if (ATwinStickGameMode* GM = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode()))
		{
			if (GM->CanSpawnNPCs())
			{
				SpawnNPC();
			}
		}
	}
}

void ATwinStickSpawner::SpawnNPC()
{
	FTransform SpawnTransform;

	// find a random point around the spawner
	FVector SpawnLoc;
	if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), SpawnLoc, SpawnRadius, NavData))
	{
		SpawnTransform.SetLocation(SpawnLoc);

		// spawn the NPC
		if (NPCClass.Num())
		{
			// TODO simplify this
			ATwinStickNPC* NPC = GetWorld()->SpawnActorDeferred<ATwinStickNPC>(NPCClass[0], SpawnTransform);
			if (IsValid(NPC))
			{
				// TODO how to get these?
				const int32 Index = FMath::RandRange(0, FMath::Clamp(EnemyDefinitions.Num() - 1, 0, EnemyDefinitions.Num() - 1));
				if (EnemyDefinitions.Num() > 0 && EnemyDefinitions[Index].Get())
				{
					NPC->SetEnemyDefinitionDataAsset(EnemyDefinitions[Index]);
				}
				NPC->FinishSpawning(NPC->GetTransform());
			}
		}
	}

	// increase the spawn counter
	++SpawnCount;

	// do we still have enemies left to spawn?
	if (SpawnCount < SpawnGroupSize)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimer, this, &ATwinStickSpawner::SpawnNPC, FMath::RandRange(MinSpawnDelay, MaxSpawnDelay), false);
	}

}
