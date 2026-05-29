// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickGameMode.h"
#include "Engine/World.h"
#include "RunDirectorSubsystem.h"
#include "TimerManager.h"
#include "UIRootWidget.h"
#include "UISubsystem.h"

void ATwinStickGameMode::BeginPlay()
{
	if (URunDirectorSubsystem* RunDirectorSubsystem = GetGameInstance()->GetSubsystem<URunDirectorSubsystem>(); IsValid(RunDirectorSubsystem))
	{
		RunDirectorSubsystem->RegisterGameMode(this);
		RunDirectorSubsystem->OnSpawnNextWave.BindUObject(this, &ATwinStickGameMode::HandleOnSpawnNextWave);
	}

	if (UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(UISubsystem))
	{
		UISubsystem->InitializeUI(UIWidgetClass);
	}
}

bool ATwinStickGameMode::CanSpawnNPCs()
{
	// is the NPC counter under the cap?
	UE_LOG(LogTemp, Warning, TEXT("NPCCount %d NPCCap %d"), NPCCount, NPCCap);
	
	const bool bWaveCleared = NPCCount < NPCCap;
	if (!bCanSpawnNextWave && bWaveCleared)
	{
		OnWaveCleared.Broadcast();
	}

	if (bCanSpawnNextWave && bWaveCleared)
	{
		UE_LOG(LogTemp, Warning, TEXT("will spawn wave"));
		bCanSpawnNextWave = false;
		return true;
	}
		
	return false;
}

void ATwinStickGameMode::IncreaseNPCs()
{
	// increase the NPC counter
	++NPCCount;
}

void ATwinStickGameMode::DecreaseNPCs()
{
	// decrease the NPC counter
	NPCCount = FMath::Max(0, --NPCCount);
}

void ATwinStickGameMode::HandleOnSpawnNextWave()
{
	bCanSpawnNextWave = true;
}

void ATwinStickGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (URunDirectorSubsystem* RunDirectorSubsystem = GetGameInstance()->GetSubsystem<URunDirectorSubsystem>(); IsValid(RunDirectorSubsystem))
	{
		RunDirectorSubsystem->UnregisterGameMode(this);
	}

	Super::EndPlay(EndPlayReason);
}