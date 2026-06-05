// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UIRootWidget.generated.h"

struct FChoiceScreenWidgetConfig;
class UChoiceScreenWidget;
class UCommonActivatableWidgetStack;

/**
 * 
 */
UCLASS()
class MLLIKE_API UUIRootWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void ShowHUD();

	UChoiceScreenWidget* ShowChoiceSelectionScreen(const FChoiceScreenWidgetConfig& Config);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonActivatableWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UChoiceScreenWidget> PerkSelectionScreenClass;
};
