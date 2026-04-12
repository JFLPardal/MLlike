// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIVFXInitData.generated.h"

USTRUCT(BlueprintType)
struct FUIVFXInitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector2D Position;
};
