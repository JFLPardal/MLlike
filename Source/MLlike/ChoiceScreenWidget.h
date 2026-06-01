// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ChoiceScreenWidget.generated.h"

DECLARE_DELEGATE(FOnChoiceMade)

/**
 * 
 */
UCLASS()
class MLLIKE_API UChoiceScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnChoiceMade OnChoiceMade;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	//TODO delete after confirmation button is added
	UPROPERTY(EditAnywhere)
	float AutoCloseTimerDuration = 1.0f;

private:
	//TODO delete after confirmation button is added
	void AutoClose();

	//TODO delete after confirmation button is added
	FTimerHandle AutoCloseHandle;
};
