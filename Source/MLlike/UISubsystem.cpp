// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "MLlikeLogCategories.h"
#include "RarityEnum.h"
#include "RarityToColorConfig.h"
#include "UIRootWidget.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FindRarityToColorConfig();
}

void UUISubsystem::InitializeUI(TSubclassOf<UUIRootWidget> WidgetClass)
{
	if (UIWidget = CreateWidget<UUIRootWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WidgetClass); IsValid(UIWidget))
	{
		UIWidget->AddToViewport(0);

		UIWidget->ShowHUD();
	}
	else
	{
		UE_LOG(LogMLlikeUI, Error, TEXT("%s - Couldn't create UIRootWidget! No UI will be shown."), TEXT(__FUNCSIG__));
	}
}

UChoiceScreenWidget* UUISubsystem::ShowChoiceSelectionScreen(const FChoiceScreenWidgetConfig& Config)
{
	if (IsValid(UIWidget))
	{
		return UIWidget->ShowChoiceSelectionScreen(Config);
	}

	return nullptr;
}

FColor UUISubsystem::GetColorForRarity(ERarity Rarity) const
{
	return RarityToColorConfig->GetColorForRarity(Rarity);
}

void UUISubsystem::FindRarityToColorConfig()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> RarityToColorConfigAssets;
	AssetRegistryModule.Get().GetAssetsByClass(URarityToColorConfig::StaticClass()->GetClassPathName(), RarityToColorConfigAssets);

	if (RarityToColorConfigAssets.IsEmpty())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - No RarityToColorConfig found - places where rarities are shown will not use differentiating colors.\nDo we have any assets inheriting from URarityToColorConfig?"), TEXT(__FUNCSIG__));
		return;
	}
	else if (RarityToColorConfigAssets.Num() > 1)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Found more than 1 possible config for RarityToColorConfig, will use [%s]"), TEXT(__FUNCSIG__), *RarityToColorConfigAssets[0].GetFullName());
	}

	TSoftObjectPtr<URarityToColorConfig> RarityToColorConfigSoftPtr = TSoftObjectPtr<URarityToColorConfig>(RarityToColorConfigAssets[0].GetSoftObjectPath());
	RarityToColorConfig = RarityToColorConfigSoftPtr.LoadSynchronous();
}
