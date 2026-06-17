// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "DamageTypeConfig.generated.h"

class UGameplayEffect;

UCLASS()
class MLLIKE_API UDamageTypeConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag DamageTypeTag;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectToApply;
};
