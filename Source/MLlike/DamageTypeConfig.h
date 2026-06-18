// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "DamageTypeConfig.generated.h"

class UGameplayEffect;

UENUM()
enum class EGameplayEffectParameterType
{
	SetByCaller,
	Period,
	Duration
};

USTRUCT(BlueprintType)
struct FGameplayEffectParameterType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere)
	EGameplayEffectParameterType ParameterType;
};

UCLASS()
class MLLIKE_API UDamageTypeConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Tag"))
	TArray<FGameplayEffectParameterType> RelevantAttributesForGameplayEffect;

public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
