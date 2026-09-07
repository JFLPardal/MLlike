// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedEffectWidget.h"

void UTimedEffectWidget::SetEffectData(const FCountdownEffectData& Data)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	//The way this is implemented, assumes only 1 status effect will be shown on the HUD
	if (!TimerManager.IsTimerActive(StatusEffectTimer))
	{
		TimerManager.SetTimer(StatusEffectTimer, this, &UTimedEffectWidget::EffectWoreOff, Data.Duration);
		BP_EffectStarted(Data);
	}
}

void UTimedEffectWidget::EffectWoreOff()
{
	BP_EffectWoreOff();
}
