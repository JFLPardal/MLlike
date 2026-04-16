// Fill out your copyright notice in the Description page of Project Settings.

#include "SegmentedBarWidget.h"

#include "BarSegmentWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "EnergyAmountChangedData.h"
#include "MaxAmmoChangedData.h"
#include "MLlikeGameplayTags.h"
#include "MLlikeLogCategories.h"
#include "MLlikeUtils.h"
#include "UIVFXInitData.h"


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

				if (UBarSegmentWidget* const SegmentToUpdate = Cast<UBarSegmentWidget>(SegmentsContainer->GetChildAt(i)); IsValid(SegmentToUpdate))
				{
					SegmentToUpdate->SetProgress(1.0f);
				}
			}
		}
		else // energy decreased
		{
			for (int i = OldActiveSegmentIndex; i > SegmentToUpdateIndex; --i)
			{
				if (UBarSegmentWidget* const SegmentToUpdate = Cast<UBarSegmentWidget>(SegmentsContainer->GetChildAt(i)); IsValid(SegmentToUpdate))
				{
					SegmentToUpdate->SetProgress(0.0f);
				}
			}
		}
	}
	
	if (UBarSegmentWidget* const SegmentToUpdate = Cast<UBarSegmentWidget>(SegmentsContainer->GetChildAt(SegmentToUpdateIndex)); IsValid(SegmentToUpdate))
	{
		if (EnergyAmountChangedData.ChangeReason == EEnergyAmountChangedReason::ShotFired)
		{
			FUIVFXInitData UIVFXInitData;
			SegmentToUpdate->PopulateVFXInitData(UIVFXInitData);

			OnPlayUIVFX.ExecuteIfBound(UIVFXInitData);
		}

		SegmentToUpdate->SetProgress(SegmentToUpdateProgress);
	}

	const int32 NumberOfFullSegments = SegmentToUpdateIndex;
	for (int i = 0; i < NumberOfFullSegments; ++i)
	{
		if (UBarSegmentWidget* const SegmentToUpdate = Cast<UBarSegmentWidget>(SegmentsContainer->GetChildAt(i)); IsValid(SegmentToUpdate))
		{
			// +1 because we want to prioritize SegmentColorB rather than A
			const float ColorAlpha = static_cast<float>(i + 1) / NumberOfFullSegments;
			SegmentToUpdate->UpdateFullColor(GetSegmentColor(ColorAlpha));
		}
	}

	UpdateSegmentsIfBarFullChanged(EnergyAmountChangedData);
}

void USegmentedBarWidget::OnMaxAmmoChanged(FGameplayTag Channel, const FMaxAmmoChangedData& EnergyAmountChangedData)
{
	SegmentsContainer->ClearChildren();

	const float EnergyPerSegment = EnergyAmountChangedData.EnergyCostPerShot;
	bool bFoundUnfilledBarSegment = false;
	const int32 NewMaxAmmo = EnergyAmountChangedData.NewMaxAmmo;
	for (int i = 0; i < NewMaxAmmo; i++)
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
			
		if (UBarSegmentWidget* const BarSegment = CreateWidget<UBarSegmentWidget>(this, SegmentClass); IsValid(BarSegment))
		{
			// ensure Segments' width changes with amount of Segments in SegmentsContainer. This makes it so that the SegmentsContainer will always have the same width, as in ML
			if (UHorizontalBoxSlot* const SegmentSlot = SegmentsContainer->AddChildToHorizontalBox(BarSegment); IsValid(SegmentSlot))
			{
				FSlateChildSize Size;
				Size.SizeRule = ESlateSizeRule::Fill;
				Size.Value = 1.0f;
				SegmentSlot->SetSize(Size);
			}
			
			// PopulateSegmentInitData
			{
				FBarSegmentInitData SegmentInitData;

				SegmentInitData.Padding = PaddingPerSegment;
				SegmentInitData.EdgeDescription = 
					(i == 0 && NewMaxAmmo == 1) ? EBarSegmentEdgeDescription::Both :
					(i == 0) ? EBarSegmentEdgeDescription::Left : 
					(i == NewMaxAmmo - 1) ? EBarSegmentEdgeDescription::Right : 
					EBarSegmentEdgeDescription::None;
				const float ColorAlpha = static_cast<float>(i) / NewMaxAmmo;
				SegmentInitData.Color = GetSegmentColor(ColorAlpha);
				SegmentInitData.ColorIfBarIsFull = SegmentColorB;

				BarSegment->Init(SegmentInitData);
			}

			BarSegment->SetProgress(SegmentProgress);
		}
	}

	InformSegmentsIfBarFullOnInit(EnergyAmountChangedData.CurrentEnergy);
}

FLinearColor USegmentedBarWidget::GetSegmentColor(float RatioOfColors) const
{
	return FMath::Lerp(SegmentColorA, SegmentColorB, RatioOfColors);
}

void USegmentedBarWidget::UpdateSegmentsIfBarFullChanged(const FEnergyAmountChangedData& EnergyAmountChangedData)
{
	const bool bBarWasFullButNoLongerIs = FMath::IsNearlyEqual(EnergyAmountChangedData.OldValue, 100.0f, UE_KINDA_SMALL_NUMBER) && !FMath::IsNearlyEqual(EnergyAmountChangedData.NewValue, 100.0f, UE_KINDA_SMALL_NUMBER);
	const bool bBarWasNotFullButIsNow = !FMath::IsNearlyEqual(EnergyAmountChangedData.OldValue, 100.0f, UE_KINDA_SMALL_NUMBER) && FMath::IsNearlyEqual(EnergyAmountChangedData.NewValue, 100.0f, UE_KINDA_SMALL_NUMBER);
	if (bBarWasFullButNoLongerIs || bBarWasNotFullButIsNow)
	{
		for (UWidget* const SegmentAsWidget : SegmentsContainer->GetAllChildren())
		{
			if (UBarSegmentWidget* const Segment = Cast<UBarSegmentWidget>(SegmentAsWidget); IsValid(Segment))
			{
				Segment->BarFullStateChanged(/*bIsBarFull*/ bBarWasNotFullButIsNow ? true : false);
			}
		}
	}	
}

void USegmentedBarWidget::InformSegmentsIfBarFullOnInit(float CurrentProgress)
{
	const bool bIsBarFull = FMath::IsNearlyEqual(CurrentProgress, 100.0f, UE_KINDA_SMALL_NUMBER);
	for (UWidget* const SegmentAsWidget : SegmentsContainer->GetAllChildren())
	{
		if (UBarSegmentWidget* const Segment = Cast<UBarSegmentWidget>(SegmentAsWidget); IsValid(Segment))
		{
			Segment->BarFullStateChanged(/*bIsBarFull*/ bIsBarFull ? true : false);
		}
	}
}

void USegmentedBarWidget::NativeDestruct()
{
	EnergyChangedHandle.Unregister();
	MaxAmmoChangedHandle.Unregister();

	Super::NativeDestruct();
}
