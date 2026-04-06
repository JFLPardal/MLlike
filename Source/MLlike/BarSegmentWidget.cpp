// Fill out your copyright notice in the Description page of Project Settings.


#include "BarSegmentWidget.h"

#include "Components/ProgressBar.h"


void UBarSegmentData::SetBarSegmentProgress(float NewProgress)
{
    const float OldProgress = m_Progress;
    m_Progress = NewProgress;
    OnBarSegmentDataChanged.ExecuteIfBound(OldProgress, m_Progress);
}

void UBarSegmentWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(ListItemObject); IsValid(SegmentData))
    {
        SegmentData->OnBarSegmentDataChanged.BindUObject(this, &UBarSegmentWidget::OnBarSegmentDataChanged);

        OnBarSegmentDataChanged(SegmentData->GetBarSegmentProgress(), SegmentData->GetBarSegmentProgress());
    }

    // call this after the rest of the logic as this will eventually call the BP event?
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UBarSegmentWidget::OnBarSegmentDataChanged(float OldProgress, float NewProgress)
{
    const bool bIsBarFull = FMath::IsNearlyEqual(NewProgress, 1.0f, UE_KINDA_SMALL_NUMBER);

    m_ProgressBar->SetFillColorAndOpacity(bIsBarFull ? m_SegmentFullColor : m_SegmentNotFullColor);
    m_ProgressBar->SetPercent(NewProgress);
}

void UBarSegmentWidget::NativeDestruct()
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(GetListItem()); IsValid(SegmentData) && SegmentData->OnBarSegmentDataChanged.IsBound())
    {
        SegmentData->OnBarSegmentDataChanged.Unbind();
    }

    Super::NativeDestruct();
}