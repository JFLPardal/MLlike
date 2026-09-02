#pragma once

#include "CoreMinimal.h"
#include "HealthBarInitData.generated.h"

class UMLLikeAbilitySystemComponent;

USTRUCT()
struct FHealthBarInitData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UMLLikeAbilitySystemComponent> m_ASC;

	UPROPERTY()
	FGameplayAttribute m_CurrentHealthAttribute;

	UPROPERTY()
	float m_MaxHealth = -1.0f;
};


