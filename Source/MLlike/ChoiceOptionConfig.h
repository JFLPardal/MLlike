// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChoiceOptionConfig.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceOptionConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// TODO make this softPtr
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;
};
