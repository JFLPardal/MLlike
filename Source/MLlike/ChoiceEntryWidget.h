// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ChoiceEntryWidget.generated.h"

class UChoiceOptionConfig;
class UCommonTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceEntryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWithConfig(const UChoiceOptionConfig* const ChoiceConfig);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Description;
};
