// Fill out your copyright notice in the Description page of Project Settings.


#include "UIRootWidget.h"

#include "ChoiceScreenWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UUIRootWidget::ShowHUD()
{
	GameStack->AddWidget<UCommonActivatableWidget>(HUDWidgetClass);
}

UChoiceScreenWidget* UUIRootWidget::ShowChoiceSelectionScreen(const FChoiceScreenWidgetConfig& Config)
{
	UChoiceScreenWidget* const ChoiceScreenWidget = ModalStack->AddWidget<UChoiceScreenWidget>(PerkSelectionScreenClass);
	ChoiceScreenWidget->InitializeWithConfig(Config);
	
	return ChoiceScreenWidget;
}
