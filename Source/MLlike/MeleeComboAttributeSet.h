// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MeleeComboAttributeSet.generated.h"

UENUM(BlueprintType)
enum class EMeleeComboStep : uint8
{
	First	UMETA(DisplayName="First"),
	Second	UMETA(DisplayName = "Second"),
	Third	UMETA(DisplayName = "Third")
};

/**
 * 
 */
UCLASS()
class MLLIKE_API UMeleeComboAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	ATTRIBUTE_ACCESSORS_BASIC(UMeleeComboAttributeSet, FirstStepDamage)
	ATTRIBUTE_ACCESSORS_BASIC(UMeleeComboAttributeSet, SecondStepDamage)
	ATTRIBUTE_ACCESSORS_BASIC(UMeleeComboAttributeSet, ThirdStepDamage)
		
public:
	UMeleeComboAttributeSet();
	
	UFUNCTION(BlueprintCallable)
	float GetDamageForStep(EMeleeComboStep Step) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData FirstStepDamage;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData SecondStepDamage;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData ThirdStepDamage;
};
