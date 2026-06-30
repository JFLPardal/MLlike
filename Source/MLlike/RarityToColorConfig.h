// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RarityEnum.h"
#include "RarityToColorConfig.generated.h"

class UCommonTextStyle;

USTRUCT(BlueprintType)
struct FRarityToColorEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ERarity Rarity;

	UPROPERTY(EditAnywhere)
	FColor Color;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonTextStyle> TextStyle;
};

/**
 * 
 */
UCLASS()
class MLLIKE_API URarityToColorConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (TitleProperty = "Rarity"))
	TArray<FRarityToColorEntry> RarityToColor;

public:
	URarityToColorConfig();

	FColor GetColorForRarity(ERarity Rarity) const;

	TSubclassOf<UCommonTextStyle> GetTextStyleForRarity(ERarity Rarity) const;
};
