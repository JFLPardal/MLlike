// Fill out your copyright notice in the Description page of Project Settings.


#include "BarSegmentWidget.h"

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

void UBarSegmentWidget::OnBarSegmentDataChanged(bool bBarSegmentActive)
{
    if (!bBarSegmentActive)
    {
        PlayAnimation(HideAnimation);
    }
}

void UBarSegmentData::SetIsBarSegmentActive(bool bIsActive)
{
    if (bIsActive != bIsBarSegmentActive)
    {
        bIsBarSegmentActive = bIsActive;
        OnBarSegmentDataChanged.ExecuteIfBound(bIsBarSegmentActive);
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