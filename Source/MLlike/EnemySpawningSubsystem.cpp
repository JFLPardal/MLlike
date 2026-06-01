// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawningSubsystem.h"

#include "EngineUtils.h"
#include "RunDirectorSubsystem.h"
#include "TwinStickSpawner.h"

static TAutoConsoleVariable<bool> CVarEnableEnemySpawn(TEXT("ml.EnableEnemySpawn"), true, TEXT(""));
static TAutoConsoleVariable<int32> CVarMaxNumEnemiesToSpawnPerWave(TEXT("ml.MaxNumEnemiesToSpawnPerWave"), 0, TEXT("if <= 0, native value will be used"));

void UEnemySpawningSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	for (TActorIterator<ATwinStickSpawner> SpawnerIt(GetWorld()); SpawnerIt; ++SpawnerIt)
	{
		Spawners.Add(*SpawnerIt);
	}

	if (URunDirectorSubsystem* RunDirectorSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URunDirectorSubsystem>(); IsValid(RunDirectorSubsystem))
	{
		RunDirectorSubsystem->OnSpawnNextWave.BindUObject(this, &UEnemySpawningSubsystem::HandleOnSpawnNextWave);

		RunDirectorSubsystem->RegisterEnemySpawningSubsystem(this);
	}
}

void UEnemySpawningSubsystem::EnemyDestroyed()
{
	NumActiveEnemies = FMath::Max(0, --NumActiveEnemies);
	UE_LOG(LogTemp, Warning, TEXT("enemy destroyed. %d/%d remaining"), NumActiveEnemies, NumMaxActiveEnemies);

	if (NumActiveEnemies == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("broadcasting OnWaveCleared"));
		OnWaveCleared.Broadcast();
	}
}

bool UEnemySpawningSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

void UEnemySpawningSubsystem::HandleOnSpawnNextWave()
{
	if (CVarEnableEnemySpawn.GetValueOnGameThread() && Spawners.Num() > 0)
	{
		NumMaxActiveEnemies = CVarMaxNumEnemiesToSpawnPerWave.GetValueOnGameThread() > 0 ? CVarMaxNumEnemiesToSpawnPerWave.GetValueOnGameThread() : NumMaxActiveEnemies;

		for (NumActiveEnemies = 0; NumActiveEnemies < NumMaxActiveEnemies; ++NumActiveEnemies)
		{
			const int32 Index = FMath::RandRange(0, FMath::Clamp(Spawners.Num() - 1, 0, Spawners.Num() - 1));
			Spawners[Index]->SpawnNPC();
		}
	}
}
