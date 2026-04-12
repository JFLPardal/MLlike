// Fill out your copyright notice in the Description page of Project Settings.


#include "UIVFXParticleManagerWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "MLlikeLogCategories.h"

void UUIVFXParticleManagerWidget::PlayFromPosition(FVector2D InitialPosition)
{		
	const int32 NumberOfParticlesToSpawn = UKismetMathLibrary::RandomIntegerInRange(MinNumberOfParticles, MaxNumberOfParticles);

	for (int32 i = 0; i < NumberOfParticlesToSpawn; ++i)
	{
		if (UUserWidget* const Particle = CreateWidget(this, ParticleWidgetClass); IsValid(Particle))
		{
			if (UCanvasPanel* const ParentAsCanvas = Cast<UCanvasPanel>(GetParent()); IsValid(ParentAsCanvas))
			{
				if (UCanvasPanelSlot* const ParticleCanvasSlot = ParentAsCanvas->AddChildToCanvas(Particle); IsValid(ParticleCanvasSlot))
				{
					FVector2D CenterMiddleAlignment{ 0.5f, 0.5f };
					ParticleCanvasSlot->SetAlignment(CenterMiddleAlignment);
					ParticleCanvasSlot->SetPosition(InitialPosition);
					ParticleCanvasSlot->SetAutoSize(true);
				}
				else
				{
					UE_LOG(LogMLlikeUI, Error, TEXT("%s - Can't cast parent slot of %s to CanvasPanelSlot - we won't play UI VFX when they were expected"), TEXT(__FUNCSIG__), *GetName());
				}
			}
		}
		else
		{
			UE_LOG(LogMLlikeUI, Error, TEXT("%s - Failed to create Particle widget with class %s - we won't see particles when they were expected"), TEXT(__FUNCSIG__), ParticleWidgetClass.Get() ? *ParticleWidgetClass->GetName() : TEXT("NO WIDGET SPECIFIED"));
		}
	}
	
}

#if WITH_EDITOR
void UUIVFXParticleManagerWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	MinNumberOfParticles = FMath::Clamp(MinNumberOfParticles, MinNumberOfParticles, MaxNumberOfParticles);
	MaxNumberOfParticles = FMath::Clamp(MaxNumberOfParticles, MinNumberOfParticles, MaxNumberOfParticles);
}
#endif
