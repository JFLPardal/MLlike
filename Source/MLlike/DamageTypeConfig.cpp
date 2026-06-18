// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTypeConfig.h"

#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult UDamageTypeConfig::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!Tag.IsValid())
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("Tag is not specified, please specify one in [%s]"), *GetName())));
		Result = EDataValidationResult::Invalid;
	}

	if (!IsValid(EffectToApply))
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("EffectToApply is not specified, please specify one in [%s]"), *GetName())));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif
