// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

#include "EnergyAmountChangedData.h"
#include "BarSegmentWidget.h"
#include "Components/ListView.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "MLlikeUtils.h"


void USegmentedBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	EnergyChangedHandle = GameplayMessageSubsystem.RegisterListener(MLlikeGameplayTags::TAG_MLlike_EnergyAmountChanged_Message, this, &USegmentedBarWidget::OnCurrentEnergyAmountChanged);
	
	// TODO - fetch this from the ammo attribute
	const int32 MaxAmmo = 5;
	for (int i = 0; i < MaxAmmo; i++)
	{
		if (UBarSegmentData* BarSegmentData = NewObject<UBarSegmentData>(this); IsValid(BarSegmentData))
		{
			// this implies bullets always start full
			BarSegmentData->SetBarSegmentProgress(1.0f);
			SegmentsList->AddItem(BarSegmentData);
		}
		else
		{
			UE_LOG(LogMLlikeUI, Error, TEXT("%s - Could not create bar segment data to add to segment bar"), TEXT(__FUNCSIG__));
		}
	}
	
}

void USegmentedBarWidget::OnCurrentEnergyAmountChanged(FGameplayTag Channel, const FEnergyAmountChangedData& EnergyAmountChangedData)
{
	// TODO get this from ShootingAttributeSet
	const int32 MaxEnergy = 100;
	const int32 EnergyPerSegment = FMath::DivideAndRoundDown(MaxEnergy, SegmentsList->GetNumItems());
	const float OldEnergyDivEnergyPerSegment = EnergyAmountChangedData.OldValue / static_cast<float>(EnergyPerSegment);
	const int32 OldActiveSegmentIndex = FMath::Floor(OldEnergyDivEnergyPerSegment);
	const float NewEnergyDivEnergyPerSegment = EnergyAmountChangedData.NewValue / static_cast<float>(EnergyPerSegment);
	const int32 SegmentToUpdateIndex = FMath::Floor(NewEnergyDivEnergyPerSegment);
	const float SegmentToUpdateProgress = FMath::Frac(NewEnergyDivEnergyPerSegment);

	if (OldActiveSegmentIndex != SegmentToUpdateIndex)
	{
		if (UBarSegmentData* const SegmentToUpdateData = Cast<UBarSegmentData>(SegmentsList->GetItemAt(OldActiveSegmentIndex)); IsValid(SegmentToUpdateData))
		{
			SegmentToUpdateData->SetBarSegmentProgress(EnergyAmountChangedData.OldValue > EnergyAmountChangedData.NewValue ? 0.0f : 1.f);
		}
	}

	if (UBarSegmentData* const SegmentToUpdateData = Cast<UBarSegmentData>(SegmentsList->GetItemAt(SegmentToUpdateIndex)); IsValid(SegmentToUpdateData))
	{
		SegmentToUpdateData->SetBarSegmentProgress(SegmentToUpdateProgress);
	}
}

void USegmentedBarWidget::NativeDestruct()
{
	EnergyChangedHandle.Unregister();
	Super::NativeDestruct();
}