// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "MLlikeLogCategories.h"
#include "UIRootWidget.h"

void UUISubsystem::InitializeUI(TSubclassOf<UUIRootWidget> WidgetClass)
{
	if (UIWidget = CreateWidget<UUIRootWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WidgetClass); IsValid(UIWidget))
	{
		UIWidget->AddToViewport(0);

		UIWidget->ShowHUD();
	}
	else
	{
		UE_LOG(LogMLlikeUI, Error, TEXT("%s - Couldn't create UIRootWidget! No UI will be shown."), TEXT(__FUNCSIG__));
	}
}

UChoiceScreenWidget* UUISubsystem::ShowPerkSelectionScreen()
{
	if (IsValid(UIWidget))
	{
		return UIWidget->ShowPerkSelectionScreen();
	}

	return nullptr;
}
