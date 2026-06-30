// Fill out your copyright notice in the Description page of Project Settings.


#include "RarityToColorConfig.h"

URarityToColorConfig::URarityToColorConfig()
	: Super()
{
	for (ERarity RarityEntry : TEnumRange<ERarity>())
	{
		RarityToColor.Add({ RarityEntry, FColor::White });
	}
}

FColor URarityToColorConfig::GetColorForRarity(ERarity Rarity) const
{
	if (const FRarityToColorEntry* const MatchedRarity = RarityToColor.FindByPredicate([Rarity](const FRarityToColorEntry& Entry) { return Entry.Rarity == Rarity; }))
	{
		return MatchedRarity->Color;
	}

	return FColor::White;
}

TSubclassOf<UCommonTextStyle> URarityToColorConfig::GetTextStyleForRarity(ERarity Rarity) const
{
	if (const FRarityToColorEntry* const MatchedRarity = RarityToColor.FindByPredicate([Rarity](const FRarityToColorEntry& Entry) { return Entry.Rarity == Rarity; }))
	{
		return MatchedRarity->TextStyle;
	}

	return nullptr;
}
