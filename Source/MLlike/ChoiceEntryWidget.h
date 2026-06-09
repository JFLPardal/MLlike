// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ChoiceEntryWidget.generated.h"


class UChoiceOptionConfig;
class UCommonTextBlock;
class UImage;

DECLARE_DELEGATE_OneParam(FOnChosen, const UChoiceOptionConfig* const);

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceEntryWidget : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	void InitializeWithConfig(const UChoiceOptionConfig* const ChoiceConfig);

	FOnChosen OnChosen;

protected:
	virtual void HandleButtonClicked() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Description;

	TObjectPtr<const UChoiceOptionConfig> Config;
};
