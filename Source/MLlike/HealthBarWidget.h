// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "HealthBarWidget.generated.h"

struct FOnAttributeChangeData;
struct FHealthBarInitData;

/**
 * 
 */
UCLASS()
class MLLIKE_API UHealthBarWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	void Init(const FHealthBarInitData& InitData);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCurrentHealthChanged(float OldValue, float NewValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHealth = -1.0f;

private:
	void OnCurrentHealthChanged(const FOnAttributeChangeData& AttributeData);
};
