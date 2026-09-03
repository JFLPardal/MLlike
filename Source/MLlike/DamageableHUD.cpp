// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableHUD.h"

#include "HealthBarInitData.h"
#include "MLlikeGameplayTags.h"

void UDamageableHUD::Init(const FDamageableHUDInitData& InitData)
{
	HealthBar->Init(InitData.HealthBarInitData);

	InitData.HealthBarInitData.m_ASC->RegisterGameplayTagEvent(MLlikeGameplayTags::TAG_MLlike_StatusEffect_Curse).AddUObject(this, &UDamageableHUD::OnCurseApplied);
}

void UDamageableHUD::OnCurseApplied(const FGameplayTag Tag, int32 Count)
{
	if (Count == 1)
	{
		CountdownEffect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if (Count == 0)
	{
		CountdownEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}
