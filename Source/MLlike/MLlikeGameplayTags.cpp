// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MLlikeGameplayTags.h"

namespace MLlikeGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(TAG_ActionTaken_Shot, "ActionTaken.Shot");

	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_Attribute_Shooting_MaxAmmo, "ShootingAttributeSet.MaxAmmo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_Attribute_Shooting_MaxEnergy, "ShootingAttributeSet.MaxEnergy");
	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_Attribute_BaseHealth_MaxHealth, "BaseHealthAttributeSet.MaxHealth");

	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_EnergyAmountChanged_Message, "EnergyAmmountChanged.Message");

	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_MaxAmmoAmountChanged_Message, "MaxAmmoAmmountChanged.Message");

	UE_DEFINE_GAMEPLAY_TAG(TAG_MLlike_Ability_Shooting_OutOfAmmo, "Abilities.Shoot.OutOfAmmo");
};

