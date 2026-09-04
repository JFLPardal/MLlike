// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableHUD.h"

#include "CountdownEffectWidget.h"
#include "HealthBarInitData.h"
#include "HealthBarWidget.h"
#include "MLLikeAbilitySystemComponent.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "UISubsystem.h"

void UDamageableHUD::Init(const FDamageableHUDInitData& InitData)
{
	OwnerASC = InitData.HealthBarInitData.m_ASC;
	HealthBar->Init(InitData.HealthBarInitData);

	if (UUISubsystem* const Subsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(Subsystem))
	{
		Subsystem->OnStatusEffectApplied.AddUObject(this, &UDamageableHUD::OnCountdownEffectApplied);
	}
}

void UDamageableHUD::OnCountdownEffectApplied(const FStatusEffectAppliedData& Data)
{
	// TODO this is being called for dead enemies. Very strange. Maybe missing unsub? But why is this still alive? In the Outliner the enemies don't exist anymore
	UE_LOG(LogTemp, Error, TEXT("OnStatusEffectApplied called for %s"), *OwnerASC->GetOwner()->GetActorNameOrLabel());
	if (Data.ASC == OwnerASC)
	{
		FCountdownEffectData CountdownEffectData;
		CountdownEffectData.Tag = Data.Tag;
		CountdownEffectData.Duration = Data.Duration;
		CountdownEffect->SetEffectData(CountdownEffectData);
	}
}
