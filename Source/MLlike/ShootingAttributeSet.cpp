// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAttributeSet.h"

#include "AmmoAmountChangedData.h"
#include "EnergyAmountChangedData.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "MLlikeGameplayTags.h"

UShootingAttributeSet::UShootingAttributeSet()
{
	// TODO: find a suitable home for this guy :)
	InitMaxAmmo(5);
	InitAmmo(GetMaxAmmo());

	InitMaxEnergy(100);
	InitEnergy(GetMaxEnergy());
}

void UShootingAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxAmmo());
	}
	else if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxEnergy());
	}
}

void UShootingAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetAmmoAttribute())
	{
		// out of ammo? 
		if (UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent(); IsValid(ASC))
		{
			(NewValue == 0) ?
				ASC->AddLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo) :
				ASC->RemoveLooseGameplayTag(MLlikeGameplayTags::TAG_MLlike_Ability_Shooting_OutOfAmmo);
		}

		if (OldValue != NewValue)
		{
			const EAmmoChangedOperation Operation = OldValue > NewValue ? EAmmoChangedOperation::AmmoUsed : EAmmoChangedOperation::AmmoAdded;
			FAmmoAmountChangedData AmmoAmountChangedData{ NewValue, Operation };
			
			UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			GameplayMessageSubsystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_AmmoAmountChanged_Message, AmmoAmountChangedData);
		}
	}
	else if (Attribute == GetEnergyAttribute())
	{
		FEnergyAmountChangedData EnergyAmountChangedData{ OldValue, NewValue };
		
		UGameplayMessageSubsystem& GameplayMessageSubystem = UGameplayMessageSubsystem::Get(GetWorld());
		GameplayMessageSubystem.BroadcastMessage(MLlikeGameplayTags::TAG_MLlike_EnergyAmountChanged_Message, EnergyAmountChangedData);
	}
}
