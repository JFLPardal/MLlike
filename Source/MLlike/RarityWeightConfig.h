// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RarityEnum.h"
#include "RarityWeightConfig.generated.h"

/**
 * Weight can be any weight, doesn't need to be between [0.0f - 1.0f ] or [0 - 100 ], the ratios of the weights is what matters for the calculation of percentage of each rarity.
 * Currently this has no protection against negative numbers
 */
UCLASS()
class MLLIKE_API URarityWeightConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FRarityToValue> RarityWeights{ {ERarity::Common, 1.0f}, {ERarity::Rare, 1.0f}, {ERarity::Epic, 1.0f}, {ERarity::Legendary, 1.0f} };

public:
	ERarity GetRandomRarity() const;
};
