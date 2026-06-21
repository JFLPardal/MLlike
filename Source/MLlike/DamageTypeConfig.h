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

USTRUCT(BlueprintType)
struct FDamageTypeUIConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	FColor IconColor = FColor::Magenta;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	FColor TextShadowColor = FColor::Magenta;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	FColor TextOutlineColor = FColor::Magenta;
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

	UPROPERTY(EditAnywhere, Category="UI")
	FDamageTypeUIConfig UIConfig;

public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
