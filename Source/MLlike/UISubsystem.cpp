// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "UIRootWidget.h"

void UUISubsystem::InitializeUI(TSubclassOf<UUIRootWidget> WidgetClass)
{
	UIWidget = CreateWidget<UUIRootWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WidgetClass);
	UIWidget->AddToViewport(0);

	UIWidget->ShowHUD();
}
