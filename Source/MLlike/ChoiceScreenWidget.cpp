// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceScreenWidget.h"

void UChoiceScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	GetWorld()->GetTimerManager().SetTimer(AutoCloseHandle, this, &UChoiceScreenWidget::AutoClose, AutoCloseTimerDuration);
}

void UChoiceScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	OnChoiceMade.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(AutoCloseHandle);
}

void UChoiceScreenWidget::AutoClose()
{
	OnChoiceMade.ExecuteIfBound();

	DeactivateWidget();
}
