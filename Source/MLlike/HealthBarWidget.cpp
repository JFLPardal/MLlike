// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

#include "MLLikeAbilitySystemComponent.h"


void UHealthBarWidget::Init(const FHealthBarInitData& InitData)
{
	MaxHealth = InitData.m_MaxHealth;

	InitData.m_ASC->GetGameplayAttributeValueChangeDelegate(InitData.m_CurrentHealthAttribute).AddUObject(this, &UHealthBarWidget::OnCurrentHealthChanged);
}

void UHealthBarWidget::OnCurrentHealthChanged(const FOnAttributeChangeData& AttributeData)
{
	BP_OnCurrentHealthChanged(AttributeData.OldValue, AttributeData.NewValue);
}

