// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

class UDamageTypeConfig;
struct FChoiceScreenWidgetConfig;
class UChoiceScreenWidget;
class UGameplayEffect;
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

	// TODO move
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayEffect> GetGameplayEffectToApplyForDamageType(FGameplayTag DamageTypeTag);

private:
	void FindRarityToColorConfig();

//TODO MOVE
	void FindDamageTypeConfigs();

private:
	TObjectPtr<UUIRootWidget> UIWidget;

	TObjectPtr<URarityToColorConfig> RarityToColorConfig;

//TODO MOVE
	TMap<FGameplayTag, const UDamageTypeConfig* const> DamageTypeTagToGameplayEffectToApply;
};
