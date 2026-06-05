// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChoiceScreenConfig.generated.h"

class UChoiceEntryWidget;
class UChoiceOptionConfig;

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceScreenConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UChoiceEntryWidget> ChoiceEntryWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UChoiceOptionConfig>> AvailableChoices;
	
};
