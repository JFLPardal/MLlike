// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickGameMode.h"
#include "UISubsystem.h"

void ATwinStickGameMode::BeginPlay()
{
	if (UUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(UISubsystem))
	{
		UISubsystem->InitializeUI(UIWidgetClass);
	}
}
