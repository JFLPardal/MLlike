// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "ShootingAttributeSet.generated.h"

/**
 * 
 */

UCLASS()
class MLLIKE_API UShootingAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, EnergyCostPerShot);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, MaxAmmo);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, Energy);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, MaxEnergy);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, ShootingFireDamage);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, ShootingFireDuration);
	ATTRIBUTE_ACCESSORS_BASIC(UShootingAttributeSet, ShootingFirePeriod);

	UShootingAttributeSet();

	void InitDependentAttributes();

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxAmmo;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Energy;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxEnergy;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData EnergyCostPerShot;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ShootingFireDamage;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ShootingFireDuration;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ShootingFirePeriod;

private:
	void UpdateOutOfAmmo();

};
