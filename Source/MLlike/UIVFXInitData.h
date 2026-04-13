// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIVFXInitData.generated.h"

USTRUCT(BlueprintType)
struct FUIVFXInitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector2D AbsolutePosition;

	/** can be limited by UUIVFXParticleManagerWidget's MinNumberOfParticles */
	UPROPERTY(BlueprintReadOnly)
	int32 MinNumberParticles = 1;

	/** can be limited by UUIVFXParticleManagerWidget's MaxNumberOfParticles */
	UPROPERTY(BlueprintReadOnly)
	int32 MaxNumberParticles = 1;
};
