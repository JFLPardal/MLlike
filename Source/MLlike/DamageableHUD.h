// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "HealthBarInitData.h"
#include "DamageableHUD.generated.h"

class UHealthBarWidget;

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
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UMLlikeWidget> CountdownEffect;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHealthBarWidget> HealthBar;
};
