// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "EnergyAmountChangedData.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "MaxAmmoChangedData.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"

UShootingAttributeSet::UShootingAttributeSet()
{
	// this is just a fallback - the true value is being specified in BP_TwinStickCharacter
	InitMaxAmmo(5);

	// this value should NEVER change. If we want more bullets, increasing MaxAmmo will adjust the cost of each shot so that we have more shots per same amount of energy
	InitMaxEnergy(100);
	InitEnergy(GetMaxEnergy());
	InitEnergyCostPerShot(FMath::DivideAndRoundDown(GetMaxEnergy(), GetMaxAmmo()));
}

void UShootingAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxEnergy());
	}
	else if (Attribute == GetMaxAmmoAttribute())
	{
		// TODO find a suitable home for this fella :)
		NewValue = FMath::Clamp(NewValue, 1, /*MaxAmmoAllowed*/ 10);
	}
}

void UShootingAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetEnergyAttribute())
	{
		UpdateOutOfAmmo();

		FEnergyAmountChangedData EnergyAmountChangedData;
		EnergyAmountChangedData.OldValue = OldValue;
		EnergyAmountChangedData.NewValue = NewValue;
		EnergyAmountChangedData.EnergyCostPerShot = GetEnergyCostPerShot();
		if (UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent(); IsValid(ASC))
		{
			if (ASC->HasMatchingGameplayTag(MLlikeGameplayTags::TAG_ActionTaken_Shot))
			{
				EnergyAmountChangedData.ChangeReason = EEnergyAmountChangedReason::ShotFired;
				ASC->RemoveLooseGameplayTag(MLlikeGameplayTags::TAG_ActionTaken_Shot);
			}
		}

		UGameplayMessageSubsystem& GameplayMessageSubystem = UGameplayMessageSubsystem::Get(GetWorld());
		GameplayMessageSubystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_EnergyAmountChanged_Message, EnergyAmountChangedData);
	}
	else
	{
		if (Attribute == GetMaxAmmoAttribute())
		{
			SetEnergyCostPerShot(FMath::DivideAndRoundDown(GetMaxEnergy(), NewValue));
				
			UpdateOutOfAmmo();

			FMaxAmmoChangedData MaxAmmoChangedData;
			MaxAmmoChangedData.CurrentEnergy = GetEnergy();
			MaxAmmoChangedData.NewMaxAmmo = NewValue;
			MaxAmmoChangedData.EnergyCostPerShot = GetEnergyCostPerShot();
			UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			GameplayMessageSubsystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_MaxAmmoAmountChanged_Message, MaxAmmoChangedData);
				
			UE_LOG(LogMLlikeGeneral, Warning, TEXT("New energy cost per shot - %f"), MaxAmmoChangedData.EnergyCostPerShot);
		}
	}
}

void UShootingAttributeSet::UpdateOutOfAmmo()
{
	if (UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent(); IsValid(ASC))
	{
		if (GetEnergy() < GetEnergyCostPerShot() && ASC->GetGameplayTagCount(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo) == 0)
		{
			ASC->AddLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo);
		}
		else if (GetEnergy() >= GetEnergyCostPerShot())
		{
			ASC->RemoveLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo);
		}
	}
}
