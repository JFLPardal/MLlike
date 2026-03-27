// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

#include "BarSegmentWidget.h"
#include "Components/ListView.h"
#include "MLlikeUtils.h"
#include "MLlikeLogCategories.h"
#include "TwinStickCharacter.h"

void USegmentedBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ATwinStickCharacter* const Character = MLlikeUtils::GetPlayerCharacter(GetWorld()); IsValid(Character))
	{
		Character->OnShotFired.AddDynamic(this, &USegmentedBarWidget::OnShotFired);
	}
	else
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - No Character was found"), TEXT(__FUNCSIG__));
	}

	// TODO - fetch this from the ammo attribute
	const int32 MaxAmmo = 5;
	for (int i = 0; i < MaxAmmo; i++)
	{
		if (UBarSegmentData* BarSegmentData = NewObject<UBarSegmentData>(this); IsValid(BarSegmentData))
		{
			//	TODO - this should be set according to initial bullets
			BarSegmentData->SetIsBarSegmentActive(true);
			SegmentsList->AddItem(BarSegmentData);
		}
		else
		{
			UE_LOG(LogMLlikeUI, Error, TEXT("%s - Could not create bar segment data to add to segment bar"), TEXT(__FUNCSIG__));
		}
	}
	
}

void USegmentedBarWidget::OnShotFired(const int32 RemainingAmmo)
{
	if (UBarSegmentData* const Data = Cast<UBarSegmentData>(SegmentsList->GetItemAt(RemainingAmmo)); IsValid(Data))
	{
		Data->SetIsBarSegmentActive(false);
	}
}

void USegmentedBarWidget::NativeDestruct()
{
	if (ATwinStickCharacter* const Character = MLlikeUtils::GetPlayerCharacter(GetWorld()); IsValid(Character))
	{
		Character->OnShotFired.RemoveDynamic(this, &USegmentedBarWidget::OnShotFired);
	}
	
	Super::NativeDestruct();
}