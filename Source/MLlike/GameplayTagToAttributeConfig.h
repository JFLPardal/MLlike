// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayTagToAttributeConfig.generated.h"


USTRUCT(BlueprintType)
struct FGameplayTagToAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag AttributeTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayAttribute Attribute;

	bool operator==(const FGameplayTagToAttribute& Other) const { return AttributeTag == Other.AttributeTag; }
	
	friend uint32 GetTypeHash(const FGameplayTagToAttribute& Value)
	{
		return GetTypeHash(Value.AttributeTag);
	}
};

/**
 * Specifies which gameplay attribute is related with which gameplay tag. 
 * This is used when populating the variables of a gameplay effect that is Perk based.
 */
UCLASS()
class MLLIKE_API UGameplayTagToAttributeConverter : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSet<FGameplayTagToAttribute> GameplayTagToAttribute;

public:
	const FGameplayAttribute* const GetAttributeForGameplayTag(FGameplayTag AttributeTag);
};
