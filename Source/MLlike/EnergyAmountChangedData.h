// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnergyAmountChangedData.generated.h"

UENUM()
enum class EEnergyAmountChangedReason
{
	ShotFired,
	Unassigned
};

USTRUCT(BlueprintType)
struct FEnergyAmountChangedData
{
	GENERATED_BODY()

	UPROPERTY()
	float OldValue = -1.0f;

	UPROPERTY()
	float NewValue = -1.0f;

	UPROPERTY()
	float EnergyCostPerShot = -1.0f;

	UPROPERTY()
	EEnergyAmountChangedReason ChangeReason = EEnergyAmountChangedReason::Unassigned;
};