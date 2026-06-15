// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

struct FChoiceScreenWidgetConfig;
class UChoiceScreenWidget;
class UUIRootWidget;
class URarityToColorConfig;
enum class ERarity : uint8;

/**
 * 
 */
UCLASS()
class MLLIKE_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void InitializeUI(TSubclassOf<UUIRootWidget> WidgetClass);

	UChoiceScreenWidget* ShowChoiceSelectionScreen(const FChoiceScreenWidgetConfig& Config);

	UFUNCTION(BlueprintCallable)
	FColor GetColorForRarity(ERarity Rarity) const;

private:
	void FindRarityToColorConfig();

private:
	TObjectPtr<UUIRootWidget> UIWidget;

	TObjectPtr<URarityToColorConfig> RarityToColorConfig;
};
