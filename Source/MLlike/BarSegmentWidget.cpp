// Fill out your copyright notice in the Description page of Project Settings.


#include "BarSegmentWidget.h"

#include "Components/ProgressBar.h"

void UBarSegmentData::SetIsBarSegmentActive(bool bIsActive)
{
    if (bIsActive != m_bIsBarSegmentActive)
    {
        m_bIsBarSegmentActive = bIsActive;
        // TODO check these values before commiting
        OnBarSegmentDataChanged.ExecuteIfBound(m_bIsBarSegmentActive, /*OldProgress*/ m_Progress, /*NewProgress*/ m_Progress);
    }
}

void UBarSegmentData::ToggleIsBarSegmentActive()
{
    SetIsBarSegmentActive(!m_bIsBarSegmentActive);
}

void UBarSegmentData::SetBarSegmentProgress(float NewProgress)
{
    const float OldProgress = m_Progress;
    m_Progress = NewProgress;
    OnBarSegmentDataChanged.ExecuteIfBound(m_bIsBarSegmentActive, OldProgress, m_Progress);
}

void UBarSegmentWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(ListItemObject); IsValid(SegmentData))
    {
        bIsBarSegmentActive = SegmentData->GetIsBarSegmentActive();
        SegmentData->OnBarSegmentDataChanged.BindUObject(this, &UBarSegmentWidget::OnBarSegmentDataChanged);
    }

    // call this after the rest of the logic as this will eventually call the BP event?
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UBarSegmentWidget::OnBarSegmentDataChanged(bool bBarSegmentActive, float OldProgress, float NewProgress)
{
    // TODO needs revisiting
    if (FMath::IsNearlyEqual(OldProgress, NewProgress, UE_KINDA_SMALL_NUMBER))
    {
        PlayAnimation(ShowHideAnimation, /*StartTime*/ 0.0f, /*NumberOfLoops*/ 1, bBarSegmentActive ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
    }
    else
    {
        m_ProgressBar->SetPercent(NewProgress);
    }
}

void UBarSegmentWidget::NativeDestruct()
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(GetListItem()); IsValid(SegmentData) && SegmentData->OnBarSegmentDataChanged.IsBound())
    {
        SegmentData->OnBarSegmentDataChanged.Unbind();
    }

    Super::NativeDestruct();
}