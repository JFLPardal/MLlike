// Fill out your copyright notice in the Description page of Project Settings.


#include "MLlikeBlueprintUtils.h"

#include "MLlikeLogCategories.h"

UActorComponent* UMLlikeBlueprintUtils::GetActorsFirstInterfaceOfType(TSubclassOf<UInterface> Interface, AActor* const Actor)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - calling with invalid Actor"), TEXT(__FUNCSIG__));
		return nullptr;
	}

	if (!Interface.Get())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - calling with invalid Interface"), TEXT(__FUNCSIG__));
		return nullptr;
	}

	const TArray<UActorComponent*> Components = Actor->GetComponentsByInterface(Interface);
	if (Components.IsEmpty())
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Actor %s has no component that implements specified Interface"), TEXT(__FUNCSIG__), *Actor->GetName());
		return nullptr;
	}

	if (Components.Num() > 1)
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("%s - Actor %s has more than one component that implements specified Interface. One of them will be used"), TEXT(__FUNCSIG__), *Actor->GetName());
	}

	return Components[0];
}