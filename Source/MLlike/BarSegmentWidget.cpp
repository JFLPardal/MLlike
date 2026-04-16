// Fill out your copyright notice in the Description page of Project Settings.


#include "BarSegmentWidget.h"

#include "UIVFXInitData.h"

void UBarSegmentWidget::Init(const FBarSegmentInitData& InitData)
{
    BP_Init(InitData);
}

void UBarSegmentWidget::PopulateVFXInitData(FUIVFXInitData& InitData) const
{
    // position to spawn FX will be the absolute position of the segment + its width
    FVector2D SegmentToSpawnFXAbsolutePosition = GetPaintSpaceGeometry().GetAbsolutePosition();
    SegmentToSpawnFXAbsolutePosition.X += GetPaintSpaceGeometry().GetAbsoluteSize().X;

    InitData.AbsolutePosition = SegmentToSpawnFXAbsolutePosition;
    InitData.MinNumberParticles = m_MinNumberVFXParticles;
    InitData.MaxNumberParticles = m_MaxNumberVFXParticles;
}

void UBarSegmentWidget::SetProgress(float NewProgress)
{
    const float OldProgress = m_Progress;
    m_Progress = NewProgress;

    const bool bWasBarFull = FMath::IsNearlyEqual(OldProgress, 1.0f, UE_KINDA_SMALL_NUMBER);

    BP_OnProgressChanged(m_Progress, bWasBarFull, IsSegmentFull());
}

void UBarSegmentWidget::UpdateFullColor(FLinearColor NewColor)
{    
    BP_UpdateSegmentFullColor(NewColor);
}

void UBarSegmentWidget::BarFullStateChanged(bool bIsBarFull)
{
    BP_BarFullStateChanged(bIsBarFull);
}

bool UBarSegmentWidget::IsSegmentFull() const
{
    return FMath::IsNearlyEqual(m_Progress, 1.0f, UE_KINDA_SMALL_NUMBER);
}
