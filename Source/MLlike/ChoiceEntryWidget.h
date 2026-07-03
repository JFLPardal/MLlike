// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RarityEnum.h"
#include "ChoiceEntryWidget.generated.h"

class UChoiceOptionConfig;
class UCommonTextBlock;
class UCommonRichTextBlock;
class UImage;
class UMLlikeButton;

DECLARE_DELEGATE_OneParam(FOnChosen, const UChoiceOptionConfig* const);

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceEntryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:	
	void InitializeWithConfigAndDelayShowing(const UChoiceOptionConfig* const ChoiceConfig, float DelayInSecs);

	FOnChosen OnChosen;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Initialize();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Show();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonRichTextBlock> Description;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMLlikeButton> Button;

	UPROPERTY(BlueprintReadOnly)
	ERarity Rarity;

	TObjectPtr<const UChoiceOptionConfig> Config;

private:
	void HandleButtonClicked();

private:
	FTimerHandle ShowTimerHandle;
};
