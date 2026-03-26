// Fill out your copyright notice in the Description page of Project Settings.

#include "MLlikeUtils.h"

#include "Kismet/GameplayStatics.h"

namespace MLlikeUtils
{
	static ATwinStickCharacter* GetPlayerCharacter(const UObject* const World)
	{
		return Cast<ATwinStickCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}
};
