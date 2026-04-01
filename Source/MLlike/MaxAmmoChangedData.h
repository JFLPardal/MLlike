// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MaxAmmoChangedData.generated.h"


USTRUCT(BlueprintType)
struct FMaxAmmoChangedData
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentEnergy = -1.0f;

	UPROPERTY()
	int32 NewMaxAmmo = -1;
};