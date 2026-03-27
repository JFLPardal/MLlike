// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoAmountChangedData.generated.h"

UENUM(BlueprintType)
enum class EAmmoChangedOperation : uint8 { NotSpecified, AmmoAdded, AmmoUsed };

USTRUCT(BlueprintType)
struct FAmmoAmountChangedData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 RemainingAmmo = -1;

	UPROPERTY(BlueprintReadOnly)
	EAmmoChangedOperation AmmoChangedOperation = EAmmoChangedOperation::NotSpecified;
};
