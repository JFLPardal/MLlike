// Fill out your copyright notice in the Description page of Project Settings.


#include "RarityWeightConfig.h"

ERarity URarityWeightConfig::GetRandomRarity() const
{
	float TotalWeight = 0.0f;
	for (const FRarityToValue& RarityWeight : RarityWeights)
	{
		TotalWeight += RarityWeight.Value;
	}

	const float RandomNumber = FMath::RandRange(0.0f, TotalWeight);
	
	UE_LOG(LogTemp, Warning, TEXT("Random number %f / %f"), RandomNumber, TotalWeight);

	float AccumulatedWeight = 0.0f;
	for (const FRarityToValue& RarityWeight : RarityWeights)
	{
		if (RandomNumber < RarityWeight.Value + AccumulatedWeight)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rarity %d"), RarityWeight.Rarity);
			return RarityWeight.Rarity;
		}
		AccumulatedWeight += RarityWeight.Value;
	}

	return ERarity::Default;
}
