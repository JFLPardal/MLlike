// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "CountdownEffectWidget.generated.h"

USTRUCT(BlueprintType)
struct FCountdownEffectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadOnly)
	float Duration;
};

/**
 * The way this is implemented, assumes only 1 status effect will be shown on the HUD
 *
 */
UCLASS()
class MLLIKE_API UCountdownEffectWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	void SetEffectData(const FCountdownEffectData& Data);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EffectStarted(const FCountdownEffectData& Data);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EffectWoreOff();

	void EffectWoreOff();

	FTimerHandle StatusEffectTimer;
};
