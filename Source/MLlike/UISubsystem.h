// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

struct FChoiceScreenWidgetConfig;
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

	UChoiceScreenWidget* ShowChoiceSelectionScreen(const FChoiceScreenWidgetConfig& Config);

private:
	TObjectPtr<UUIRootWidget> UIWidget;
};
