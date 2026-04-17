// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthAttributeSet.h"

#include "GameFramework/GameplayMessageSubsystem.h"

UBaseHealthAttributeSet::UBaseHealthAttributeSet()
{
	// TODO move this
	const float LocalMaxHealth = 2.0f;
	InitMaxHealth(LocalMaxHealth);
	InitCurrentHealth(GetMaxHealth());
}

void UBaseHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		// TODO move this
		const float LocalMaxHealth = 2.0f;
		NewValue = FMath::Clamp(NewValue, 0.0f, LocalMaxHealth);
	}
}
