// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnergyAmountChangedData.generated.h"

USTRUCT(BlueprintType)
struct FEnergyAmountChangedData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 OldValue = -1;

	UPROPERTY()
	int32 NewValue = -1;
};