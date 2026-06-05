// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawningSubsystem.h"

#include "Algo/RandomShuffle.h"
#include "EngineUtils.h"
#include "MLlikeLogCategories.h"
#include "RunDirectorSubsystem.h"
#include "TwinStickSpawner.h"

static TAutoConsoleVariable<bool> CVarEnableEnemySpawn(TEXT("ml.EnableEnemySpawn"), true, TEXT(""));
static TAutoConsoleVariable<int32> CVarMaxNumEnemiesToSpawnPerWave(TEXT("ml.MaxNumEnemiesToSpawnPerWave"), 0, TEXT("if <= 0, native value will be used"));

static FAutoConsoleCommandWithWorld ClearWaveCommand(
	TEXT("ML.ClearWave"),
	TEXT(""),
	FConsoleCommandWithWorldDelegate::CreateStatic(
	[](UWorld* World)
	{
		if (!World)
		{
			return;
		}

		if (UEnemySpawningSubsystem* const EnemySpawningSubsystem = World->GetSubsystem<UEnemySpawningSubsystem>(); IsValid(EnemySpawningSubsystem))
		{
			EnemySpawningSubsystem->DebugClearWave();
		}
	})
);

void UEnemySpawningSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	for (TActorIterator<ATwinStickSpawner> SpawnerIt(GetWorld()); SpawnerIt; ++SpawnerIt)
	{
		AllSpawners.Add(*SpawnerIt);
	}

	UnusedSpawners.Reserve(AllSpawners.Num());
	UnusedSpawners = AllSpawners;
	Algo::RandomShuffle(UnusedSpawners);

	if (URunDirectorSubsystem* RunDirectorSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URunDirectorSubsystem>(); IsValid(RunDirectorSubsystem))
	{
		RunDirectorSubsystem->OnSpawnNextWave.BindUObject(this, &UEnemySpawningSubsystem::HandleOnSpawnNextWave);

		RunDirectorSubsystem->RegisterEnemySpawningSubsystem(this);
	}
}

void UEnemySpawningSubsystem::EnemyDestroyed()
{
	NumRemaningEnemiesInWave = FMath::Max(0, --NumRemaningEnemiesInWave);
	UE_LOG(LogMLlikeGeneral, Warning, TEXT("enemy destroyed. %d/%d remaining"), NumRemaningEnemiesInWave, NumMaxEnemiesPerWave);

	if (NumRemaningEnemiesInWave == 0)
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("broadcasting OnWaveCleared"));

		UnusedSpawners = AllSpawners;
		Algo::RandomShuffle(UnusedSpawners);

		OnWaveCleared.Broadcast();
	}
}

bool UEnemySpawningSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

void UEnemySpawningSubsystem::HandleOnSpawnNextWave()
{
	const int32 NumSpawners = AllSpawners.Num();
	if (CVarEnableEnemySpawn.GetValueOnGameThread() && NumSpawners > 0)
	{
		NumMaxEnemiesPerWave = CVarMaxNumEnemiesToSpawnPerWave.GetValueOnGameThread() > 0 ? CVarMaxNumEnemiesToSpawnPerWave.GetValueOnGameThread() : NumMaxEnemiesPerWave;

		if (NumMaxEnemiesPerWave > NumSpawners)
		{
			UE_LOG(LogMLlikeGeneral, Error, TEXT("Not enough ATwinStickSpawner in the level [%d] to spawn [%d] Enemies. Only [%d] enemies will be spawned"), NumSpawners, NumMaxEnemiesPerWave, NumSpawners);
		}

		for (NumRemaningEnemiesInWave = 0; NumRemaningEnemiesInWave < NumMaxEnemiesPerWave && UnusedSpawners.Num() > 0; ++NumRemaningEnemiesInWave)
		{
			UnusedSpawners.Pop(EAllowShrinking::No)->SpawnNPC();
		}
	}
}

#if !UE_BUILD_SHIPPING
void UEnemySpawningSubsystem::DebugClearWave()
{
	for (TActorIterator<ATwinStickNPC> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
	{
		EnemyIt->DebugKill();
	}
}
#endif
