// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthAttributeSet.h"

#include "GameplayEffectExtension.h"

UBaseHealthAttributeSet::UBaseHealthAttributeSet()
{
	InitDamage(0.0f);
}

void UBaseHealthAttributeSet::InitDependentAttributes()
{
	SetCurrentHealth(GetMaxHealth());
}

void UBaseHealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (!FMath::IsNearlyZero(GetDamage(), UE_KINDA_SMALL_NUMBER))
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetDamage(), 0.0f, GetMaxHealth()));
			SetDamage(0.0f);
		}
	}
}
