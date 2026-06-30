// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"

#include "AbilitySystemComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CommonTextBlock.h"
#include "DamageTypeConfig.h"
#include "GameplayTagToAttributeConfig.h"
#include "Kismet/GameplayStatics.h"
#include "MLlikeLogCategories.h"
#include "RarityEnum.h"
#include "RarityToColorConfig.h"
#include "UIRootWidget.h"

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FindRarityToColorConfig();

	FindDamageTypeConfigs();

	FindGameplayTagsToGameplayAttributesConfig();
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

TSubclassOf<UCommonTextStyle> UUISubsystem::GetTextStyleForRarity(ERarity Rarity) const
{
	return RarityToColorConfig->GetTextStyleForRarity(Rarity);
}

FColor UUISubsystem::GetColorForRarity(ERarity Rarity) const
{
	return RarityToColorConfig->GetColorForRarity(Rarity);
}

void UUISubsystem::ApplyGameplayEffectForDamageType(FGameplayTag DamageTypeTag, UAbilitySystemComponent* const Source, UAbilitySystemComponent* const Target)
{
	if (!IsValid(Source))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Invalid Source AbilitySystemComponent was given with DamageTypeTag [%s]. No GameplayEffect will be applied"), 
											TEXT(__FUNCSIG__),
											*DamageTypeTag.ToString());
		return;
	}

	if (!IsValid(Target))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Invalid Target AbilitySystemComponent was given with DamageTypeTag [%s]. No GameplayEffect will be applied"),
											TEXT(__FUNCSIG__),
											*DamageTypeTag.ToString());
		return;
	}

	const UDamageTypeConfig* const* DamageTypeConfig = DamageTypeTagToGameplayEffectToApply.Find(DamageTypeTag);
	if (DamageTypeConfig == nullptr)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Can't find UDamageTypeConfig for damage type tag [%s]. No GameplayEffect will be applied"),
											TEXT(__FUNCSIG__),
											*DamageTypeTag.ToString());
		return;
	}
	
	TSubclassOf<UGameplayEffect> GESubclassToApply = (*DamageTypeConfig)->EffectToApply;
	if (!IsValid(GESubclassToApply))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Did not find GameplayEffect to Apply for tag [%s] - is it missing from the DataAsset inheriting from UDamageTypeConfig? No GameplayEffect will be applied"), 
											TEXT(__FUNCSIG__),
											*DamageTypeTag.ToString());
		return;
	}

	FGameplayEffectSpecHandle SpecHandle = Source->MakeOutgoingSpec(GESubclassToApply, /*Level*/1, Source->MakeEffectContext());
	for (const FGameplayEffectParameterType& RelevantAttributeForGameplayEffect : (*DamageTypeConfig)->RelevantAttributesForGameplayEffect)
	{
		const FGameplayAttribute* const AttributeForGameplayTag = GameplayTagsToGameplayAttributesConverter->GetAttributeForGameplayTag(RelevantAttributeForGameplayEffect.Tag);
		if (AttributeForGameplayTag == nullptr)
		{
			UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - what to write here? No GameplayEffect will be applied"),
				TEXT(__FUNCSIG__));
		}
		else
		{
			const float AttributeValueToSet = Source->GetNumericAttribute(*AttributeForGameplayTag);
			switch (RelevantAttributeForGameplayEffect.ParameterType)
			{
			case EGameplayEffectParameterType::Duration:
			{
				SpecHandle.Data->SetDuration(AttributeValueToSet, /*bLockDuration*/ true);
				break;
			}
			case EGameplayEffectParameterType::Period:
			{
				SpecHandle.Data->Period = AttributeValueToSet;
				break;
			}
			case EGameplayEffectParameterType::SetByCaller:
			{
				SpecHandle.Data->SetSetByCallerMagnitude(RelevantAttributeForGameplayEffect.Tag, AttributeValueToSet);
				break;
			}
			}
		}
	}

	Source->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, Target);
}

bool UUISubsystem::GetUIConfigForDamageType(FGameplayTag DamageTypeTag, FDamageTypeUIConfig& UIConfig) const
{
	const UDamageTypeConfig* const* DamageTypeConfig = DamageTypeTagToGameplayEffectToApply.Find(DamageTypeTag);
	if (DamageTypeConfig == nullptr)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Can't find UDamageTypeConfig for damage type tag [%s]. UIConfig for damage type cannot be retrieved"),
			TEXT(__FUNCSIG__),
			*DamageTypeTag.ToString());
		return false;
	}

	UIConfig = (*DamageTypeConfig)->UIConfig;
	return true;
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

void UUISubsystem::FindDamageTypeConfigs()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> DamageTypeConfigAssets;
	AssetRegistryModule.Get().GetAssetsByClass(UDamageTypeConfig::StaticClass()->GetClassPathName(), DamageTypeConfigAssets);

	if (DamageTypeConfigAssets.IsEmpty())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - No DamageTypeConfig found - DamageType effects won't apply any damage.\nDo we have any assets inheriting from UDamageTypeConfig?"), TEXT(__FUNCSIG__));
		return;
	}

	for (const FAssetData& DamageTypeConfigAsset : DamageTypeConfigAssets)
	{
		TSoftObjectPtr<UDamageTypeConfig> DamageTypeConfigSoftPtr = TSoftObjectPtr<UDamageTypeConfig>(DamageTypeConfigAsset.GetSoftObjectPath());
		if (UDamageTypeConfig* DamageTypeConfig = DamageTypeConfigSoftPtr.LoadSynchronous(); IsValid(DamageTypeConfig))
		{
			DamageTypeTagToGameplayEffectToApply.Add({ DamageTypeConfig->Tag, DamageTypeConfig });
		}
	}
}

void UUISubsystem::FindGameplayTagsToGameplayAttributesConfig()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> GameplayTagToAttributeConverterAssets;
	AssetRegistryModule.Get().GetAssetsByClass(UGameplayTagToAttributeConverter::StaticClass()->GetClassPathName(), GameplayTagToAttributeConverterAssets);

	if (GameplayTagToAttributeConverterAssets.IsEmpty())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - No GameplayTagToAttributeConfig found - won't be able to apply some perk effects\nDo we have any assets inheriting from UGameplayTagToAttributeConfig?"), TEXT(__FUNCSIG__));
		return;
	}
	else if (GameplayTagToAttributeConverterAssets.Num() > 1)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Found more than 1 possible config for UGameplayTagToAttributeConfig, will use [%s]"), TEXT(__FUNCSIG__), *GameplayTagToAttributeConverterAssets[0].GetFullName());
	}

	TSoftObjectPtr<UGameplayTagToAttributeConverter> GameplayTagToAttributeConverterSoftPtr = TSoftObjectPtr<UGameplayTagToAttributeConverter>(GameplayTagToAttributeConverterAssets[0].GetSoftObjectPath());
	GameplayTagsToGameplayAttributesConverter = GameplayTagToAttributeConverterSoftPtr.LoadSynchronous();
}
