// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ChoiceScreenWidget.generated.h"

DECLARE_DELEGATE(FOnChoiceMade)

class UChoiceEntryWidget;
class UChoiceOptionConfig;
class UCommonButtonBase;
class UPanelWidget;

USTRUCT()
struct FChoiceScreenWidgetConfig
{
	GENERATED_BODY()

	FChoiceScreenWidgetConfig();
	FChoiceScreenWidgetConfig(TArray<const UChoiceOptionConfig* const>& InChoicesToShow, TSubclassOf<UChoiceEntryWidget> InChoiceEntryWidgetClass);

	UPROPERTY()
	TArray<const UChoiceOptionConfig* const> ChoicesToShow;

	UPROPERTY()
	TSubclassOf<UChoiceEntryWidget> ChoiceEntryWidgetClass;
};

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnChoiceMade OnChoiceMade;

	void InitializeWithConfig(const FChoiceScreenWidgetConfig& Config);

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonButtonBase> m_ConfirmButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPanelWidget> m_ChoicesList = nullptr;

	bool bChoicesInitialized = false;

private:
	void HandleOnConfirmButtonClicked();

	void Cleanup();
};
