// Fill out your copyright notice in the Description page of Project Settings.


#include "UIRootWidget.h"

#include "Widgets/CommonActivatableWidgetContainer.h"

void UUIRootWidget::ShowHUD()
{
	GameStack->AddWidget<UCommonActivatableWidget>(HUDWidgetClass);
}
