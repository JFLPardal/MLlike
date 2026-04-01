// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ShootingAttributeSet.generated.h"

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
	FGameplayAttributeData MaxAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Energy;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxEnergy;

	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, MaxAmmo);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, MaxEnergy);

};
