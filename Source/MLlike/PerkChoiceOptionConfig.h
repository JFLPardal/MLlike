// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChoiceOptionConfig.h"
#include "GameplayTagContainer.h"
#include "PerkChoiceOptionConfig.generated.h"

class UGameplayEffect;

/**
 * 
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
};
