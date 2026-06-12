// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkChoiceOptionConfig.h"

#include "GameplayEffect.h"
#include "Misc/DataValidation.h"


FText UPerkChoiceOptionConfig::GetDescription() const
{
	FFormatNamedArguments Args;
	for (const FPerkParameter& PerkParameter : PerkParameters)
	{
		// TODO get level here as last arg
		Args.Add(PerkParameter.DescriptionArgumentName.ToString(), PerkParameter.Magnitude.GetValueAtLevel(1));
	}

	return FText::Format(Description, Args);
}

EDataValidationResult UPerkChoiceOptionConfig::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!IsValid(GameplayEffectToGrant))
	{
		return EDataValidationResult::Invalid;
	}

	// check for dup tags in GameplayEffectMagnitudes
	TSet<FGameplayTag> GameplayEffectMagnitudesTags;
	for (const FPerkParameter& GEMagnitude : PerkParameters)
	{
		if (!GEMagnitude.DataTag.IsValid())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid tag set in GameplayEffectMagnitudes for [%s] - Did you forget to add a tag?"),
																	*GetName())));
			Result = EDataValidationResult::Invalid;
		}
		else
		{
			if (!GameplayEffectMagnitudesTags.Add(GEMagnitude.DataTag).IsValidId())
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Adding duplicated tag [%s] to GameplayEffectToGrant in [%s] - Please remove duplicate"),
																		*GEMagnitude.DataTag.ToString(),
																		*GetName())));
				Result = EDataValidationResult::Invalid;
			}
		}
	}

	const TArray<FGameplayModifierInfo>& ModifiersInfo = GameplayEffectToGrant->GetDefaultObject<UGameplayEffect>()->Modifiers;
	for (const FGameplayModifierInfo& Modifier : ModifiersInfo)
	{
		if (Modifier.ModifierMagnitude.GetMagnitudeCalculationType() == EGameplayEffectMagnitudeCalculation::SetByCaller)
		{
			// check if ModifierDataTag is specified in GameplayEffectMagnitudes and remove it from the set. This will help make sure that after this loop, no GameplayEffectMagnitudes was defined that doesn't exist
			FGameplayTag ModifierDataTag = Modifier.ModifierMagnitude.GetSetByCallerFloat().DataTag;
			if (!GameplayEffectMagnitudesTags.Remove(ModifierDataTag))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("GameplayEffectMagnitudesTags in [%s] is not specifying tag needed [%s] for [%s] - please add it"),
																		*GetName(), 
																		*ModifierDataTag.ToString(), 
																		*GameplayEffectToGrant->GetName())));
				Result = EDataValidationResult::Invalid;
			}
		}
		else
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Modifier [%s] in [%s] in [%s] need to have MagnitudeCalculationType set to `SetByCaller` for Perks"), 
																	*Modifier.Attribute.GetName(), 
																	*GameplayEffectToGrant->GetName(), 
																	*GetName())));
			Result = EDataValidationResult::Invalid;
		}
	}

	// checks if all tags specified in GameplayEffectMagnitudes are needed for the gameplay effect
	if (!GameplayEffectMagnitudesTags.IsEmpty())
	{
		FString UnusedTags;
		for (const FGameplayTag& Tag : GameplayEffectMagnitudesTags)
		{
			UnusedTags += Tag.ToString() + " ";
		}

		Context.AddError(FText::FromString(FString::Printf(TEXT("Specified tags [%s] in GameplayEffectMagnitudes for [%s] that are not used by [%s] - please remove them"), 
																*UnusedTags, 
																*GetName(), 
																*GameplayEffectToGrant->GetName())));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}
