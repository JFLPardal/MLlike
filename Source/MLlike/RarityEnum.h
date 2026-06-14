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
	Common		UMETA(DisplayName = "Common"),
	Rare		UMETA(DisplayName = "Rare"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DisplayName = "Legendary"),
	
	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(ERarity, ERarity::Count);

USTRUCT(BlueprintType)
struct FRarityToValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERarity Rarity;

	UPROPERTY(EditAnywhere)
	float Value;
};