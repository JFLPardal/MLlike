// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

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
		if (UUserWidget* const Segment = CreateWidget(SegmentsList, SegmentsList->GetEntryWidgetClass()); IsValid(Segment))
		{
			SegmentsList->AddItem(Segment);
		}
		else
		{
			UE_LOG(LogMLlikeUI, Error, TEXT("%s - Could not create segment to add to segment bar"), TEXT(__FUNCSIG__));
		}
	}
	
}

void USegmentedBarWidget::OnShotFired(const int32 RemainingAmmo)
{
	BP_OnShotFired(RemainingAmmo);
}

void USegmentedBarWidget::NativeDestruct()
{
	if (ATwinStickCharacter* const Character = MLlikeUtils::GetPlayerCharacter(GetWorld()); IsValid(Character))
	{
		Character->OnShotFired.RemoveDynamic(this, &USegmentedBarWidget::OnShotFired);
	}
	
	Super::NativeDestruct();
}