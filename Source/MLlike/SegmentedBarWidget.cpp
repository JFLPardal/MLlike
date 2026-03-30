// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

#include "BarSegmentWidget.h"
#include "Components/ListView.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "MLlikeUtils.h"


void USegmentedBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	AmmoChangedHandle = GameplayMessageSubsystem.RegisterListener(MLlikeGameplayTags::TAG_MLlike_AmmoAmountChanged_Message, this, &USegmentedBarWidget::OnCurrentAmmoAmountChanged);

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

void USegmentedBarWidget::OnCurrentAmmoAmountChanged(FGameplayTag Channel, const FAmmoAmountChangedData& AmmoAmountChangedData)
{
	if (Channel == MLlikeGameplayTags::TAG_MLlike_AmmoAmountChanged_Message)
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
}

void USegmentedBarWidget::NativeDestruct()
{
	AmmoChangedHandle.Unregister();

	Super::NativeDestruct();
}