// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class MLLIKE_API UBaseHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseHealthAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(UBaseHealthAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseHealthAttributeSet, CurrentHealth);

protected:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData CurrentHealth;
};
