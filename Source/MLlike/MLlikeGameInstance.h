// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MLlikeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MLLIKE_API UMLlikeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

private:
#if !UE_BUILD_SHIPPING
	void ApplyLocalDevSettings();
#endif
};
