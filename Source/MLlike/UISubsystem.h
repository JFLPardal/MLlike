// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

class UAbilitySystemComponent;
class UDamageTypeConfig;
struct FChoiceScreenWidgetConfig;
class UChoiceScreenWidget;
struct FDamageTypeUIConfig;
class UGameplayEffect;
class UGameplayTagToAttributeConverter;
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

	// TODO move
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UCommonTextStyle> GetTextStyleForRarity(ERarity Rarity) const;

	// TODO move
	UFUNCTION(BlueprintCallable)
	FColor GetColorForRarity(ERarity Rarity) const;

	// TODO move
	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectForDamageType(FGameplayTag DamageTypeTag, UAbilitySystemComponent* const Source, UAbilitySystemComponent* const Target);
	
	// TODO move
	// returns true if DamageTypeTag exists
	UFUNCTION(BlueprintCallable)
	bool GetUIConfigForDamageType(FGameplayTag DamageTypeTag, FDamageTypeUIConfig& UIConfig) const;

private:
	void FindRarityToColorConfig();

//TODO MOVE
	void FindDamageTypeConfigs();

//TODO move
	void FindGameplayTagsToGameplayAttributesConfig();

private:
	TObjectPtr<UUIRootWidget> UIWidget;

	TObjectPtr<URarityToColorConfig> RarityToColorConfig;
	TObjectPtr<UGameplayTagToAttributeConverter> GameplayTagsToGameplayAttributesConverter;

//TODO MOVE
	TMap<FGameplayTag, const UDamageTypeConfig* const> DamageTypeTagToGameplayEffectToApply;
};
