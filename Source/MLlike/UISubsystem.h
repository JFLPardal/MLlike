// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

class UChoiceScreenWidget;
class UUIRootWidget;

/**
 * 
 */
UCLASS()
class MLLIKE_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeUI(TSubclassOf<UUIRootWidget> WidgetClass);

	UChoiceScreenWidget* ShowPerkSelectionScreen();

private:
	TObjectPtr<UUIRootWidget> UIWidget;
};
