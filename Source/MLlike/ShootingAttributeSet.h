// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ShootingAttributeSet.generated.h"

DECLARE_DELEGATE_OneParam(FASOnShotFired, int32 /*RemainingAmmo*/);

/**
 * 
 */
UCLASS()
class MLLIKE_API UShootingAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UShootingAttributeSet();

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxAmmo;

	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, Ammo);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, MaxAmmo);

	FASOnShotFired OnShotFired;
};
