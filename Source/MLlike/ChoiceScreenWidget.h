// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ChoiceScreenWidget.generated.h"

class UChoiceEntryWidget;
class UChoiceOptionConfig;
class UCommonButtonBase;
class UPanelWidget;

DECLARE_DELEGATE_OneParam(FOnChoiceMade, const UChoiceOptionConfig* const)

USTRUCT(BlueprintType)
struct FChoiceScreenWidgetConfig
{
	GENERATED_BODY()

	FChoiceScreenWidgetConfig();
	FChoiceScreenWidgetConfig(TSubclassOf<UChoiceEntryWidget> InChoiceEntryWidgetClass, TArray<const UChoiceOptionConfig* const>& InChoicesToShow);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChoiceEntryWidget> ChoiceEntryWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<const UChoiceOptionConfig* const> ChoicesToShow;
};

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	FOnChoiceMade OnChoiceMade;

	void InitializeWithConfig(const FChoiceScreenWidgetConfig& Config);

protected:
	virtual void NativeOnDeactivated() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UPanelWidget> m_ChoicesList = nullptr;

	UPROPERTY(EditAnywhere, Category="DesignerViewTesting")
	FMargin PaddingForEntries { 0, 5 };

	bool bChoicesInitialized = false;

	UPROPERTY(EditAnywhere, Category="DesignerViewTesting")
	FChoiceScreenWidgetConfig DesignerTestingConfig;

private:
	void HandleOnChoiceChosen(const UChoiceOptionConfig* const ChosenConfig);

	void Cleanup();
};
