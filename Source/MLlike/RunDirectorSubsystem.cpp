// Fill out your copyright notice in the Description page of Project Settings.


#include "RunDirectorSubsystem.h"

#include "AbilitySystemComponent.h"
#include "Algo/RandomShuffle.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/World.h"
#include "ChoiceScreenConfig.h"
#include "ChoiceScreenWidget.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "MLlikeUtils.h"
#include "PerkChoiceOptionConfig.h"
#include "UISubsystem.h"
#include "EnemySpawningSubsystem.h"

void URunDirectorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FindPerkSelectionScreenConfig();
}

void URunDirectorSubsystem::FindPerkSelectionScreenConfig()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> PerkSelectionScreenOptionsAssets;
	//TODO how to make sure we only get perk selection screen config?
	AssetRegistryModule.Get().GetAssetsByClass(UChoiceScreenConfig::StaticClass()->GetClassPathName(), PerkSelectionScreenOptionsAssets);
	for (const FAssetData& PerkSelectionScreenOptionAsset : PerkSelectionScreenOptionsAssets)
	{
		PerkSelectionScreenConfig.Add(TSoftObjectPtr<UChoiceScreenConfig>(PerkSelectionScreenOptionAsset.GetSoftObjectPath()));
	}

	if (PerkSelectionScreenConfig.IsEmpty())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - No perks option configs found - PerkSelectionScreen will not be shown.\nDo we have any assets inheriting from UChoiceOptionConfig?"), TEXT(__FUNCSIG__));
	}
	else if (PerkSelectionScreenConfig.Num() > 1)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Found more than 1 possible config for perk selection screen"), TEXT(__FUNCSIG__));
	}
}

void URunDirectorSubsystem::HandleWaveCleared()
{
	if (UUISubsystem* const UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(UISubsystem) && !PerkSelectionScreenConfig.IsEmpty())
	{
		if (UnchosenPerks.IsEmpty())
		{
			if (!bInitializedUnchosenPerks && PerkSelectionScreenConfig[0].LoadSynchronous())
			{
				UnchosenPerks = PerkSelectionScreenConfig[0]->AvailableChoices;
				bInitializedUnchosenPerks = true;
			}
			else
			{
				OnSpawnNextWave.ExecuteIfBound();
				return;
			}
		}

		TArray<const UChoiceOptionConfig* const> ChoicesToShow;
		const int32 NumChoicesToShow = FMath::Min(3, UnchosenPerks.Num());
		Algo::RandomShuffle(UnchosenPerks);
		for (int i = 0; i < NumChoicesToShow; ++i)
		{
			ChoicesToShow.Add(UnchosenPerks[i]);
		}

		FChoiceScreenWidgetConfig PerkScreenWidgetConfig(PerkSelectionScreenConfig[0]->ChoiceEntryWidgetClass, ChoicesToShow);

		if (UChoiceScreenWidget* ChoiceScreen = UISubsystem->ShowChoiceSelectionScreen(PerkScreenWidgetConfig); IsValid(ChoiceScreen))
		{
			ChoiceScreen->OnChoiceMade.BindUObject(this, &URunDirectorSubsystem::HandleChoiceMade); 
		}
	}
}

void URunDirectorSubsystem::HandleChoiceMade(const UChoiceOptionConfig* const ChosenConfig)
{
	if (!IsValid(ChosenConfig))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Trying to grant an invalid Perk - Perk won't be granted"), TEXT(__FUNCSIG__));
		return;

	}
	
	const UPerkChoiceOptionConfig* const PerkConfig = Cast<UPerkChoiceOptionConfig>(ChosenConfig);
	if (!IsValid(PerkConfig))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Trying to grant an invalid Perk - choice option config selected [%s] must inherit from UPerkChoiceOptionConfig, it's currently inheriting from [%s]. Perk won't be granted"), 
			TEXT(__FUNCSIG__),
			*ChosenConfig->GetName(),
			*ChosenConfig->GetClass()->GetName());
		return;
	}


	if (UAbilitySystemComponent* const PlayerAbilitySystemComponent = MLlikeUtils::GetPlayerAbilitySystemComponent(GetGameInstance()->GetWorld()); IsValid(PlayerAbilitySystemComponent))
	{
		const int32 Level = 1;
		FGameplayEffectSpecHandle SpecHandle = PlayerAbilitySystemComponent->MakeOutgoingSpec(PerkConfig->GameplayEffectToGrant, Level, PlayerAbilitySystemComponent->MakeEffectContext());
		for (const FPerkParameter& GEMagnitude : PerkConfig->PerkParameters)
		{
			SpecHandle.Data->SetSetByCallerMagnitude(GEMagnitude.DataTag, GEMagnitude.Magnitude.GetValueAtLevel(Level));
		}
		PlayerAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}

	// TODO delete when effects are being properly applied
	UE_LOG(LogMLlikeGeneral, Warning, TEXT("Option Chosen - %s / TagToGrant - %s / gameplayeffect name - %s"), 
		*PerkConfig->GetName(), 
		*PerkConfig->TagToGrant.ToString(),
		*PerkConfig->GameplayEffectToGrant->GetName());

	OnSpawnNextWave.ExecuteIfBound();
}

void URunDirectorSubsystem::RegisterEnemySpawningSubsystem(UEnemySpawningSubsystem* const EnemySpawningSubsystem)
{
	if (!IsValid(EnemySpawningSubsystem))
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - EnemySpawningSubsystem is not valid - won't be able to be informed when the wave is cleared which will affect perk screen / spawning new waves"), TEXT(__FUNCSIG__));
		return;
	}

	OnWaveClearedHandle = EnemySpawningSubsystem->OnWaveCleared.AddUObject(this, &URunDirectorSubsystem::HandleWaveCleared);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {OnSpawnNextWave.ExecuteIfBound(); });
	
}

void URunDirectorSubsystem::UnregisterEnemySpawningSubsystem(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (UEnemySpawningSubsystem* const EnemySpawningSubsystem = GetWorld()->GetSubsystem<UEnemySpawningSubsystem>(); IsValid(EnemySpawningSubsystem))
	{
		EnemySpawningSubsystem->OnWaveCleared.Remove(OnWaveClearedHandle);
	}
	else
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - EnemySpawningSubsystem is not valid - won't be able to unregister any events that were previously registered"), TEXT(__FUNCSIG__));
		return;
	}
}
