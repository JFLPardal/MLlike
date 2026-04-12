// Fill out your copyright notice in the Description page of Project Settings.


#include "BarSegmentWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/ProgressBar.h"
#include "UIVFXInitData.h"

void UBarSegmentData::SetBarSegmentProgress(float NewProgress)
{
    const float OldProgress = m_Progress;
    m_Progress = NewProgress;
    OnBarSegmentDataChanged.ExecuteIfBound(OldProgress, m_Progress);
}

void UBarSegmentWidget::PopulateVFXInitData(FUIVFXInitData& InitData) const
{
    FVector2D SegmentToSpawnFXPixelPosition, SegmentToSpawnFXViewportPosition;
    // position to spawn FX will be the absolute position of the segment + its width
    FVector2D SegmentToSpawnFXAbsolutePosition = GetPaintSpaceGeometry().GetAbsolutePosition();
    SegmentToSpawnFXAbsolutePosition.X += GetPaintSpaceGeometry().GetAbsoluteSize().X;

    USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), SegmentToSpawnFXAbsolutePosition, SegmentToSpawnFXPixelPosition, SegmentToSpawnFXViewportPosition);

    InitData.Position = SegmentToSpawnFXViewportPosition;
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
    const bool bWasBarFull = FMath::IsNearlyEqual(OldProgress, 1.0f, UE_KINDA_SMALL_NUMBER);

    BP_OnProgressChanged(NewProgress, bWasBarFull, bIsBarFull);
}

void UBarSegmentWidget::NativeDestruct()
{
    if (UBarSegmentData* const SegmentData = Cast<UBarSegmentData>(GetListItem()); IsValid(SegmentData) && SegmentData->OnBarSegmentDataChanged.IsBound())
    {
        SegmentData->OnBarSegmentDataChanged.Unbind();
    }

    Super::NativeDestruct();
}