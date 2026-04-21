// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthAttributeSet.h"

#include "GameFramework/GameplayMessageSubsystem.h"

void UBaseHealthAttributeSet::InitDependentAttributes()
{
	SetCurrentHealth(GetMaxHealth());
}

void UBaseHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}
