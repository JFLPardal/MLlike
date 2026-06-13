// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChoiceOptionConfig.h"
#include "RarityEnum.h"
#include "GameplayTagContainer.h"
#include "PerkChoiceOptionConfig.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FPerkParameter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag DataTag;

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Rarity"))
	TArray<FRarityToValue> MagnitudePerRarity{ {ERarity::Common, 1}, {ERarity::Rare, 1}, {ERarity::Epic, 1}, {ERarity::Legendary, 1} };

	UPROPERTY(EditAnywhere)
	FName DescriptionArgumentName;

	int32 GetMagnitudeForRarity(ERarity Rarity) const;
};

/**
 * includes validation for 
 * (1) GameplayEffectToGrant specified
 * (2) duplicated tags in GameplayEffectMagnitudes
 * (3) all necessary 'SetByCaller' tags are specified in GameplayEffectMagnitudes
 * (4) GameplayEffectMagnitudes only has the tags used in GameplayEffectToGrant ( no extra tags )
 * (5) ensure entries in GameplayEffectMagnitudes have a valid tag
 */
UCLASS()
class MLLIKE_API UPerkChoiceOptionConfig : public UChoiceOptionConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag TagToGrant;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectToGrant;

	UPROPERTY(EditAnywhere)
	TArray<FPerkParameter> PerkParameters;

public:
	virtual FText GetDescription() const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
