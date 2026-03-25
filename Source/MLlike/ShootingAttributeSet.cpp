// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAttributeSet.h"

UShootingAttributeSet::UShootingAttributeSet()
{
	// TODO: find a suitable home for this guy :)
	InitMaxAmmo(5);
	InitAmmo(GetMaxAmmo());
}

void UShootingAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxAmmo());
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
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Abilities.Shoot.OutOfAmmo")) :
				ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Abilities.Shoot.OutOfAmmo"));
		}
	}
}
