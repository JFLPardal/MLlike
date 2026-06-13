// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceOptionConfig.h"

#include "MLlikeLogCategories.h"

ERarity UChoiceOptionConfig::GetRarity() const
{
	return Rarity;
}

void UChoiceOptionConfig::SetRarity(ERarity InRarity)
{
	Rarity = InRarity;
}