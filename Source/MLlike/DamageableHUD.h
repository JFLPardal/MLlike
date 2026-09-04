// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "HealthBarInitData.h"
#include "DamageableHUD.generated.h"

class UCountdownEffectWidget;
class UMLLikeAbilitySystemComponent;
class UHealthBarWidget;
struct FStatusEffectAppliedData;

/**
 * 
 */

USTRUCT()
struct FDamageableHUDInitData
{
	GENERATED_BODY()

	UPROPERTY()
	FHealthBarInitData HealthBarInitData;
};

UCLASS()
class MLLIKE_API UDamageableHUD : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	void Init(const FDamageableHUDInitData& InitData);

protected:
	// TODO if only 1 countdown effect is allowed at any given time, this could be moved to UCountdownEffectWidget
	void OnCountdownEffectApplied(const FStatusEffectAppliedData& Data);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCountdownEffectWidget> CountdownEffect;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHealthBarWidget> HealthBar;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMLLikeAbilitySystemComponent> OwnerASC;
};
