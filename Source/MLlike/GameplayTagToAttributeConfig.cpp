// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagToAttributeConfig.h"

const FGameplayAttribute* const UGameplayTagToAttributeConverter::GetAttributeForGameplayTag(FGameplayTag AttributeTag)
{
	return &GameplayTagToAttribute.Find(FGameplayTagToAttribute{ AttributeTag })->Attribute;
	/*if (FSetElementId SetElementId = GameplayTagToAttribute.FindId(FGameplayTagToAttribute{ AttributeTag }); SetElementId.IsValidId())
	{
		return &GameplayTagToAttribute.Get(SetElementId).Attribute;
	}*/

	//return nullptr;
}
