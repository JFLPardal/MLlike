// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableHUD.h"

#include "HealthBarInitData.h"
#include "HealthBarWidget.h"
#include "MLLikeAbilitySystemComponent.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "TimedEffectWidget.h"
#include "UISubsystem.h"

void UDamageableHUD::Init(const FDamageableHUDInitData& InitData)
{
	OwnerASC = InitData.HealthBarInitData.m_ASC;
	HealthBar->Init(InitData.HealthBarInitData);

	if (UUISubsystem* const Subsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(Subsystem))
	{
		StatusEffectAppliedHandle = Subsystem->OnStatusEffectApplied.AddUObject(this, &UDamageableHUD::OnCountdownEffectApplied);
	}
}

void UDamageableHUD::OnCountdownEffectApplied(const FStatusEffectAppliedData& Data)
{
	if (Data.ASC == OwnerASC)
	{
		FCountdownEffectData CountdownEffectData;
		CountdownEffectData.Tag = Data.Tag;
		CountdownEffectData.Duration = Data.Duration;
		TimedEffect->SetEffectData(CountdownEffectData);
	}
}

void UDamageableHUD::NativeDestruct()
{
	if (UUISubsystem* const Subsystem = GetGameInstance()->GetSubsystem<UUISubsystem>(); IsValid(Subsystem))
	{
		Subsystem->OnStatusEffectApplied.Remove(StatusEffectAppliedHandle);
	}

	Super::NativeDestruct();
}
