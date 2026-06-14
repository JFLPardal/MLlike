// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkChoiceOptionConfig.h"

#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "MLlikeLogCategories.h"
#include "UObject/EnumProperty.h"

FText UPerkChoiceOptionConfig::GetDescription() const
{
	FFormatNamedArguments Args;
	for (const FPerkParameter& PerkParameter : PerkParameters)
	{
		Args.Add(PerkParameter.DescriptionArgumentName.ToString(), PerkParameter.GetMagnitudeForRarity(GetRarity()));
	}

	return FText::Format(Description, Args);
}

int32 FPerkParameter::GetMagnitudeForRarity(ERarity Rarity) const
{
	for (const FRarityToValue& RarityToValueEntry : MagnitudePerRarity)
	{
		if (RarityToValueEntry.Rarity == Rarity)
		{
			return RarityToValueEntry.Value;
		}
	}

	UE_LOG(LogMLlikeGeneral, Error, TEXT("Requesting Magnitude for an unspecified Rarity for PerkParameter with Description Text [%s]. Common rarity will be used"), *DescriptionArgumentName.ToString());

	return 1;
}

EDataValidationResult UPerkChoiceOptionConfig::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!IsValid(GameplayEffectToGrant))
	{
		return EDataValidationResult::Invalid;
	}

	// check for dup DescriptionArgumentName in PerkParameters
	TSet<FName> PerkParamatersDescriptionArgumentNames;
	// check for dup tags in PerkParameters
	TSet<FGameplayTag> PerkParamatersDataTags;
	// check for dup rarities in MagnitudePerRarity
	TSet<ERarity> PerkParametersRarities;
	for (const FPerkParameter& PerkParameter : PerkParameters)
	{
		// PerkParameter.DataTag validation 
		if (!PerkParameter.DataTag.IsValid())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid tag set in PerkParameters for [%s] - Did you forget to add a tag?"),
																	*GetName())));
			Result = EDataValidationResult::Invalid;
		}
		else
		{
			bool bDataTagAlreadyDefined = false;
			PerkParamatersDataTags.Add(PerkParameter.DataTag, &bDataTagAlreadyDefined);
			if(bDataTagAlreadyDefined)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Adding duplicated tag [%s] to [%s] - Please remove duplicate"),
																		*PerkParameter.DataTag.ToString(),
																		*GetName())));
				Result = EDataValidationResult::Invalid;
			}
		}

		// PerkParameter.DescriptionArgumentName validation
		if (PerkParameter.DescriptionArgumentName.IsNone())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("DescriptionArgumentName in [%s] for one of the PerkParameters is none, please specify it"),
																	*GetName())));
			Result = EDataValidationResult::Invalid;
		}
		else
		{
			bool bDescriptionArgumentNameAlreadyDefined = false;
			PerkParamatersDescriptionArgumentNames.Add(PerkParameter.DescriptionArgumentName, &bDescriptionArgumentNameAlreadyDefined);
			if(bDescriptionArgumentNameAlreadyDefined)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Adding duplicated DescriptionArgumentNames [%s] to [%s] - Please remove duplicate"),
																		*PerkParameter.DescriptionArgumentName.ToString(),
																		*GetName())));
				Result = EDataValidationResult::Invalid;
			}
		}

		// PerkParameter.MagnitudePerRarity validation
		for (const FRarityToValue& MagnitudePerRarity : PerkParameter.MagnitudePerRarity)
		{
			bool bRarityAlreadyDefined = false;
			PerkParametersRarities.Add(MagnitudePerRarity.Rarity, &bRarityAlreadyDefined);
			if (bRarityAlreadyDefined)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Adding duplicated Rarity [%s] in MagnitudePerRarity for PerkParameters with DataTag [%s] on [%s] - Please remove duplicate"),
																		*StaticEnum<ERarity>()->GetDisplayNameTextByValue(static_cast<uint64>(MagnitudePerRarity.Rarity)).ToString(),
																		*PerkParameter.DataTag.ToString(),
																		*GetName())));
				Result = EDataValidationResult::Invalid;
			}
		}

		for (ERarity RarityEntry : TEnumRange<ERarity>())
		{
			if (const bool bRarityMissing = PerkParametersRarities.Remove(RarityEntry) == 0)
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Rarity [%s] is missing in MagnitudePerRarity for PerkParameter with DataTag [%s] on [%s]. Please add the missing rarity"),
																		*StaticEnum<ERarity>()->GetDisplayNameTextByValue(static_cast<uint64>(RarityEntry)).ToString(),
																		*PerkParameter.DataTag.ToString(),
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
			// check if ModifierDataTag is specified in PerkParameters and remove it from the set. This will help make sure that after this loop, no PerkParameters was defined that doesn't exist
			FGameplayTag ModifierDataTag = Modifier.ModifierMagnitude.GetSetByCallerFloat().DataTag;
			if (!PerkParamatersDataTags.Remove(ModifierDataTag))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("PerkParamatersDataTags in [%s] is not specifying tag needed [%s] for [%s] - please add it"),
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
	if (!PerkParamatersDataTags.IsEmpty())
	{
		FString UnusedTags;
		for (const FGameplayTag& Tag : PerkParamatersDataTags)
		{
			UnusedTags += Tag.ToString() + " ";
		}

		Context.AddError(FText::FromString(FString::Printf(TEXT("Specified tags [%s] in GameplayEffectMagnitudes for [%s] that are not used by GameplayEffectToGrant [%s] - please remove them or update GameplayEffectToGrant to use them"), 
																*UnusedTags, 
																*GetName(), 
																*GameplayEffectToGrant->GetName())));
		Result = EDataValidationResult::Invalid;
	}

	TArray<FString> NamedParametersInDescription;
	FText::GetFormatPatternParameters(FTextFormat(Description), NamedParametersInDescription);
	for (const FName& PerkParamatersDescriptionArgumentName : PerkParamatersDescriptionArgumentNames)
	{
		if (!NamedParametersInDescription.Contains(PerkParamatersDescriptionArgumentName.ToString()))
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Specified DescriptionArgumentName [%s] that is unused by Description's text [%s] in [%s]. Either update the DescriptionArgumentName to match the Description, remove the PerkParameter defining it or update Description"),
				*PerkParamatersDescriptionArgumentName.ToString(),
				*Description.ToString(),
				*GetName())));
			Result = EDataValidationResult::Invalid;
		}
		else
		{
			NamedParametersInDescription.RemoveSwap(PerkParamatersDescriptionArgumentName.ToString());
		}
	}

	if (!NamedParametersInDescription.IsEmpty())
	{
		FString UnusedNamedParametersInDescription;
		for (const FString& NamedParameterInDescription : NamedParametersInDescription)
		{
			UnusedNamedParametersInDescription += NamedParameterInDescription + " ";
		}
		Context.AddError(FText::FromString(FString::Printf(TEXT("Description [%s] has named arguments [%s] that were not specified by the PerkParameters' DescriptionArgumentName. Either add the PerkParameter for the missing arguments or update Description in [%s]"),
			*Description.ToString(),
			*UnusedNamedParametersInDescription,
			*GetName())));
		Result = EDataValidationResult::Invalid;
	}

	
		
	return Result;
}

