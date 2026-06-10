// Fill out your copyright notice in the Description page of Project Settings.

#include "MLlikeUtils.h"

#include "Kismet/GameplayStatics.h"
#include "TwinStickCharacter.h"

static TAutoConsoleVariable<bool> CVarHideTutorialHints(
	TEXT("ML.HideTutorialHints"),
	false,
	TEXT("Hide tutorial helper actors")
);

namespace MLlikeUtils
{
	static ATwinStickCharacter* GetPlayerCharacter(const UObject* const World)
	{
		return Cast<ATwinStickCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}

	UAbilitySystemComponent* const GetPlayerAbilitySystemComponent(const UObject* const World)
	{
		if (ATwinStickCharacter* const PlayerCharacter = GetPlayerCharacter(World); IsValid(PlayerCharacter))
		{
			return PlayerCharacter->GetAbilitySystemComponent();
		}

		return nullptr;
	}

};
