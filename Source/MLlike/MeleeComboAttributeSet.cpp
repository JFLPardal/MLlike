// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeComboAttributeSet.h"

#include "MLlikeLogCategories.h"

UMeleeComboAttributeSet::UMeleeComboAttributeSet()
{
	InitFirstStepDamage(1.0f);
	InitSecondStepDamage(2.0f);
	InitThirdStepDamage(5.0f);
}

float UMeleeComboAttributeSet::GetDamageForStep(EMeleeComboStep Step) const
{
	switch (Step)
	{
	case EMeleeComboStep::First:
		return GetFirstStepDamage();
	case EMeleeComboStep::Second:
		return GetSecondStepDamage();
	case EMeleeComboStep::Third:
		return GetThirdStepDamage();
	}

	const float DefaultStepDamage = 1.0f;
	UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Couldn't find definition in UMeleeComboAttributeSet::ComboStepToDamage for Step [%s] - [%f] will be used as a default."),
										TEXT(__FUNCSIG__),
										*StaticEnum<EMeleeComboStep>()->GetDisplayNameTextByValue(static_cast<uint64>(Step)).ToString(),
										DefaultStepDamage);
	return DefaultStepDamage;
}
