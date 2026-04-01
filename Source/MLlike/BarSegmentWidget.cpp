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
    }

    // call this after the rest of the logic as this will eventually call the BP event?
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}
UE_DISABLE_OPTIMIZATION
void UBarSegmentWidget::OnBarSegmentDataChanged(float OldProgress, float NewProgress)
{
    if(FMath::IsNearlyEqual(NewProgress, 1.0f, UE_KINDA_SMALL_NUMBER))
    {
        if (!FMath::IsNearlyEqual(OldProgress, 1.0f, UE_KINDA_SMALL_NUMBER))
        {
            PlayAnimation(SegmentFullAnimation);
        }
        m_ProgressBar->SetPercent(NewProgress);
    }
    else if (!FMath::IsNearlyEqual(NewProgress, 1.0f, UE_KINDA_SMALL_NUMBER))
    {
        if (FMath::IsNearlyEqual(OldProgress, 1.0f, UE_KINDA_SMALL_NUMBER))
        {
            PlayAnimation(SegmentFullAnimation, /*StartTime*/ 0.0f, /*NumberOfLoops*/ 1, EUMGSequencePlayMode::Reverse);
        }
        m_ProgressBar->SetPercent(NewProgress);
    }
}
UE_ENABLE_OPTIMIZATION
void UBarSegmentWidget::NativeDestruct()
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(GetListItem()); IsValid(SegmentData) && SegmentData->OnBarSegmentDataChanged.IsBound())
    {
        SegmentData->OnBarSegmentDataChanged.Unbind();
    }

    Super::NativeDestruct();
}