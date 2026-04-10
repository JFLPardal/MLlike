// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

#include "BarSegmentWidget.h"
#include "Components/ListView.h"
#include "EnergyAmountChangedData.h"
#include "MaxAmmoChangedData.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "MLlikeUtils.h"


void USegmentedBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	EnergyChangedHandle = GameplayMessageSubsystem.RegisterListener(MLlikeGameplayTags::TAG_MLlike_EnergyAmountChanged_Message, this, &USegmentedBarWidget::OnCurrentEnergyAmountChanged);
	MaxAmmoChangedHandle = GameplayMessageSubsystem.RegisterListener(MLlikeGameplayTags::TAG_MLlike_MaxAmmoAmountChanged_Message, this, &USegmentedBarWidget::OnMaxAmmoChanged);	
}

void USegmentedBarWidget::OnCurrentEnergyAmountChanged(FGameplayTag Channel, const FEnergyAmountChangedData& EnergyAmountChangedData)
{
	const float EnergyPerSegment = EnergyAmountChangedData.EnergyCostPerShot;
	const float OldEnergyDivEnergyPerSegment = EnergyAmountChangedData.OldValue / EnergyPerSegment;
	const int32 OldActiveSegmentIndex = FMath::Floor(OldEnergyDivEnergyPerSegment);
	const float NewEnergyDivEnergyPerSegment = EnergyAmountChangedData.NewValue / EnergyPerSegment;
	const int32 SegmentToUpdateIndex = FMath::Floor(NewEnergyDivEnergyPerSegment);
	const float SegmentToUpdateProgress = FMath::Frac(NewEnergyDivEnergyPerSegment);

	// if energy increases/decreases by more than 1 segment, we need to update the segments in between
	if (OldActiveSegmentIndex != SegmentToUpdateIndex)
	{
		if (const bool EnergyIncreased = OldActiveSegmentIndex < SegmentToUpdateIndex)
		{
			for (int i = OldActiveSegmentIndex; i < SegmentToUpdateIndex; ++i)
			{
				if (UBarSegmentData* const SegmentToUpdateData = Cast<UBarSegmentData>(SegmentsList->GetItemAt(i)); IsValid(SegmentToUpdateData))
				{
					SegmentToUpdateData->SetBarSegmentProgress(1.0f);
				}
			}
		}
		else // energy decreased
		{
			for (int i = OldActiveSegmentIndex; i > SegmentToUpdateIndex; --i)
			{
				if (UBarSegmentData* const SegmentToUpdateData = Cast<UBarSegmentData>(SegmentsList->GetItemAt(i)); IsValid(SegmentToUpdateData))
				{
					SegmentToUpdateData->SetBarSegmentProgress(0.0f);
				}
			}
		}
	}

	if (UBarSegmentData* const SegmentToUpdateData = Cast<UBarSegmentData>(SegmentsList->GetItemAt(SegmentToUpdateIndex)); IsValid(SegmentToUpdateData))
	{
		if (EnergyAmountChangedData.ChangeReason == EEnergyAmountChangedReason::ShotFired)
		{
			if (UUserWidget* const SegmentToUpdateEntry = SegmentsList->GetEntryWidgetFromItem(SegmentToUpdateData); IsValid(SegmentToUpdateEntry))
			{
				FUIVFXInitData UIVFXInitData;
				UIVFXInitData.Position = SegmentToUpdateEntry->GetPaintSpaceGeometry().GetAbsolutePosition();

				OnPlayUIVFX.ExecuteIfBound(UIVFXInitData);
			}
		}

		SegmentToUpdateData->SetBarSegmentProgress(SegmentToUpdateProgress);
	}
}

void USegmentedBarWidget::OnMaxAmmoChanged(FGameplayTag Channel, const FMaxAmmoChangedData& EnergyAmountChangedData)
{
	SegmentsList->ClearListItems();

	const float EnergyPerSegment = EnergyAmountChangedData.EnergyCostPerShot;
	bool bFoundUnfilledBarSegment = false;
	for (int i = 0; i < EnergyAmountChangedData.NewMaxAmmo; i++)
	{
		if (UBarSegmentData* BarSegmentData = NewObject<UBarSegmentData>(this); IsValid(BarSegmentData))
		{
			float SegmentProgress = 0.0f;
			if (!bFoundUnfilledBarSegment)
			{
				if (EnergyAmountChangedData.CurrentEnergy >= (i + 1) * EnergyPerSegment)
				{
					SegmentProgress = 1.0f;
				}
				else
				{
					SegmentProgress = FMath::Frac((EnergyAmountChangedData.CurrentEnergy - EnergyPerSegment * i) / EnergyPerSegment);
					bFoundUnfilledBarSegment = true;
				}
			}
			BarSegmentData->SetBarSegmentProgress(SegmentProgress);
			SegmentsList->AddItem(BarSegmentData);
		}
		else
		{
			UE_LOG(LogMLlikeUI, Error, TEXT("%s - Could not create bar segment data to add to segment bar"), TEXT(__FUNCSIG__));
		}
	}
}

void USegmentedBarWidget::NativeDestruct()
{
	EnergyChangedHandle.Unregister();
	MaxAmmoChangedHandle.Unregister();

	Super::NativeDestruct();
}
