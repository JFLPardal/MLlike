// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RarityEnum.h"
#include "ChoiceOptionConfig.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceOptionConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// TODO make this softPtr
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;
	 
public:
	virtual FText GetDescription() const { return Description; }

	ERarity GetRarity() const;

	//Rarity defines how strong the effect to apply will be. Common is the weakest, Legendary the strongest ( not enforced in DataValidation ).
	void SetRarity(ERarity InRarity);

private:
	ERarity Rarity{ ERarity::Common };
};
