// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UIRootWidget.generated.h"

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonActivatableWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> ModalStack;
};
