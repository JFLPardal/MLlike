// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ATwinStickCharacter;
class UAbilitySystemComponent;

namespace MLlikeUtils
{
	static ATwinStickCharacter* GetPlayerCharacter(const UObject* const World);
	static UAbilitySystemComponent* const GetPlayerAbilitySystemComponent(const UObject* const World);
};
