// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RarityEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ERarity : uint8 
{
	Common,
	Rare,
	Epic,
	Legendary,

	Default = Common
};

USTRUCT(BlueprintType)
struct FRarityToValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERarity Rarity;

	UPROPERTY(EditAnywhere)
	float Value;
};