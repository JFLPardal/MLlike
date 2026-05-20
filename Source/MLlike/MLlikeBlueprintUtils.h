// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MLlikeBlueprintUtils.generated.h"

class AActor;
class UActorComponent;
class UInterface;

/**
 * 
 */
UCLASS()
class MLLIKE_API UMLlikeBlueprintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static UActorComponent* GetActorsFirstInterfaceOfType(TSubclassOf<UInterface> Interface, AActor* const Actor);
};
