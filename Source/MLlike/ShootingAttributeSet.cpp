// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "EnergyAmountChangedData.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "MaxAmmoChangedData.h"
#include "MLlikeGameplayTags.h"

UShootingAttributeSet::UShootingAttributeSet()
{
	// TODO: find a suitable home for this guy :)
	InitMaxAmmo(5);

	InitMaxEnergy(100);
	InitEnergy(GetMaxEnergy());
}

void UShootingAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxEnergy());
	}
	else if (Attribute == GetMaxAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1, /*MaxAmmoAllowed*/ 10);
	}
}

void UShootingAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetEnergyAttribute())
	{
		// out of ammo? 
		if (UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent(); IsValid(ASC))
		{
			(GetEnergy() < GetMaxEnergy() / GetMaxAmmo() && ASC->GetGameplayTagCount(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo) == 0) ?
				ASC->AddLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo) :
				ASC->RemoveLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo);
		}

		FEnergyAmountChangedData EnergyAmountChangedData;
		EnergyAmountChangedData.OldValue = OldValue;
		EnergyAmountChangedData.NewValue = NewValue;
		
		UGameplayMessageSubsystem& GameplayMessageSubystem = UGameplayMessageSubsystem::Get(GetWorld());
		GameplayMessageSubystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_EnergyAmountChanged_Message, EnergyAmountChangedData);
	}
	else
	{
		if (Attribute == GetMaxAmmoAttribute())
		{
			if (OldValue != NewValue)
			{
				FMaxAmmoChangedData MaxAmmoChangedData;
				MaxAmmoChangedData.CurrentEnergy = GetEnergy();
				MaxAmmoChangedData.NewMaxAmmo = NewValue;
				
				UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
				GameplayMessageSubsystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_MaxAmmoAmountChanged_Message, MaxAmmoChangedData);
			}
		}
	}
}
