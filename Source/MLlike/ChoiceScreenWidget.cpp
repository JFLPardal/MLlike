// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceScreenWidget.h"

#include "CommonButtonBase.h"

void UChoiceScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	m_ConfirmButton->OnClicked().AddUObject(this, &UChoiceScreenWidget::HandleOnConfirmButtonClicked);
}

void UChoiceScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	OnChoiceMade.Unbind();
}

void UChoiceScreenWidget::HandleOnConfirmButtonClicked()
{
	OnChoiceMade.ExecuteIfBound();

	DeactivateWidget();
}
