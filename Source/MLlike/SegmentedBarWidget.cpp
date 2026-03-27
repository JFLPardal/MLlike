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
		Character->OnCurrentAmmoAmountChanged.AddDynamic(this, &USegmentedBarWidget::OnCurrentAmmoAmountChanged);
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

void USegmentedBarWidget::OnCurrentAmmoAmountChanged(const FAmmoAmountChangedData AmmoAmountChangedData)
{
	if (AmmoAmountChangedData.AmmoChangedOperation == EAmmoChangedOperation::NotSpecified || AmmoAmountChangedData.RemainingAmmo < 0)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - FAmmoAmountChangedData was not properly set - Can't update widget"), TEXT(__FUNCSIG__));
		return;
	}

	const int32 InvalidIndex = -1;
	const int32 SegmentToUpdateIndex =
		AmmoAmountChangedData.AmmoChangedOperation == EAmmoChangedOperation::AmmoAdded ?
		AmmoAmountChangedData.RemainingAmmo -1 : AmmoAmountChangedData.AmmoChangedOperation == EAmmoChangedOperation::AmmoUsed ?
		AmmoAmountChangedData.RemainingAmmo : InvalidIndex;

	if (SegmentToUpdateIndex == InvalidIndex)
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - FAmmoAmountChangedData was not properly set - Can't update widget"), TEXT(__FUNCSIG__));
		return;
	}

	if (UBarSegmentData* const Data = Cast<UBarSegmentData>(SegmentsList->GetItemAt(SegmentToUpdateIndex)); IsValid(Data))
	{
		Data->ToggleIsBarSegmentActive();
	}
}

void USegmentedBarWidget::NativeDestruct()
{
	if (ATwinStickCharacter* const Character = MLlikeUtils::GetPlayerCharacter(GetWorld()); IsValid(Character))
	{
		Character->OnCurrentAmmoAmountChanged.RemoveDynamic(this, &USegmentedBarWidget::OnCurrentAmmoAmountChanged);
	}
	
	Super::NativeDestruct();
}