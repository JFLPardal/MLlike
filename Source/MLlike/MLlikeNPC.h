// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Variant_TwinStick/AI/TwinStickNPC.h"
#include "MLlikeNPC.generated.h"

class UMLLikeAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MLLIKE_API AMLlikeNPC : public ATwinStickNPC
{
	GENERATED_BODY()

public:
	AMLlikeNPC();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMLLikeAbilitySystemComponent> m_ASC = nullptr;
	
};
