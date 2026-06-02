// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickSpawner.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/AssetData.h"
#include "EnemyDefinitionDataAsset.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastNavMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TwinStickNPC.h"

ATwinStickSpawner::ATwinStickSpawner()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void ATwinStickSpawner::BeginPlay()
{
	Super::BeginPlay();

	InitializeEnemyDefinitions();

	// find the recast navmesh actor on the level
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecastNavMesh::StaticClass(), ActorList);

	if (ActorList.Num() > 0)
	{
		NavData = Cast<ARecastNavMesh>(ActorList[0]);
	} else {

		UE_LOG(LogTemp, Log, TEXT("Could not find recast navmesh"));

	}
}

void ATwinStickSpawner::SpawnNPC()
{
	FTransform SpawnTransform;

	// find a random point around the spawner
	FVector CurrentActorLocation = GetActorLocation() + FVector(0.0f, 0.0f, SpawnRadius);
	FVector RandomSpawnLocation = CurrentActorLocation;
	while (RandomSpawnLocation == CurrentActorLocation)
	{
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), RandomSpawnLocation, SpawnRadius, NavData);
	}

	SpawnTransform.SetLocation(RandomSpawnLocation);

	ATwinStickNPC* NPC = GetWorld()->SpawnActorDeferred<ATwinStickNPC>(NPCClass, SpawnTransform);
	if (IsValid(NPC))
	{
		if (EnemyDefinitions.Num() > 0)
		{
			const int32 Index = FMath::RandRange(0, FMath::Clamp(EnemyDefinitions.Num() - 1, 0, EnemyDefinitions.Num() - 1));
			NPC->SetEnemyDefinitionDataAsset(EnemyDefinitions[Index].LoadSynchronous());
		}
		NPC->FinishSpawning(NPC->GetTransform());
	}
}

void ATwinStickSpawner::InitializeEnemyDefinitions()
{
	EnemyDefinitions.Empty();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> EnemyDefinitionAssets;
	AssetRegistryModule.Get().GetAssetsByClass(UEnemyDefinitionDataAsset::StaticClass()->GetClassPathName(), EnemyDefinitionAssets);
	for (const FAssetData& EnemyDefinitionAsset : EnemyDefinitionAssets)
	{
		EnemyDefinitions.Add(TSoftObjectPtr<UEnemyDefinitionDataAsset>(EnemyDefinitionAsset.ToSoftObjectPath()));
	}
}
