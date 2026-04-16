// Fill out your copyright notice in the Description page of Project Settings.


#include "UIVFXParticleManagerWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "MLlikeLogCategories.h"
#include "TimerManager.h"
#include "UIVFXInitData.h"

void UUIVFXParticleManagerWidget::PlayParticles(const FUIVFXInitData& ParticlesInitData)
{
	const int32 MinNumberOfParticlesToSpawn = FMath::Clamp(ParticlesInitData.MinNumberParticles, MinNumberOfParticles, ParticlesInitData.MinNumberParticles);
	const int32 MaxNumberOfParticlesToSpawn = FMath::Clamp(ParticlesInitData.MaxNumberParticles, ParticlesInitData.MaxNumberParticles, MaxNumberOfParticles);

	if (ParticlesInitData.MaxNumberParticles > MaxNumberOfParticles || ParticlesInitData.MinNumberParticles < MinNumberOfParticles)
	{
		UE_LOG(LogMLlikeUI, Error, TEXT("%s - Defined number of UIVFXParticles to be spawned [%d,%d] does not respect values set in this widget [%d,%d] - Spawned UIVFXParticles will be restricted to this range"), 
			TEXT(__FUNCSIG__), 
			ParticlesInitData.MinNumberParticles, ParticlesInitData.MaxNumberParticles,
			MinNumberOfParticles, MaxNumberOfParticles);
	}

	const int32 NumberOfParticlesToSpawn = UKismetMathLibrary::RandomIntegerInRange(MinNumberOfParticlesToSpawn, MaxNumberOfParticlesToSpawn);

	// this implies that if the particles are spawned fast enough that the 'previous' spawning didn't finish when the current one starts, not all previous particles with be spawned
	m_State.InitData = ParticlesInitData;
	m_State.NumberOfParticlesSpawned = 0;
	m_State.NumberOfParticlesToSpawn = NumberOfParticlesToSpawn;

	FTimerManagerTimerParameters TimerParameters;
	TimerParameters.bLoop = true;
	TimerParameters.bMaxOncePerFrame = true;
	
	TimerManager = &GetWorld()->GetTimerManager();
	TimerManager->SetTimer(CreateParticleTimerHandle, this, &UUIVFXParticleManagerWidget::CheckIfShouldSpawnParticleAndSpawnIt, 0.0001f, TimerParameters);
}

void UUIVFXParticleManagerWidget::CreateParticle(const FUIVFXInitData& ParticlesInitData)
{
	if (UUserWidget* const Particle = CreateWidget(this, ParticleWidgetClass); IsValid(Particle))
	{
		if (UCanvasPanel* const ParentAsCanvas = Cast<UCanvasPanel>(GetParent()); IsValid(ParentAsCanvas))
		{
			if (UCanvasPanelSlot* const ParticleCanvasSlot = ParentAsCanvas->AddChildToCanvas(Particle); IsValid(ParticleCanvasSlot))
			{
				FVector2D CenterMiddleAlignment{ 0.5f, 0.5f };
				ParticleCanvasSlot->SetAlignment(CenterMiddleAlignment);

				FVector2D SegmentToSpawnFXPixelPosition, SegmentToSpawnFXViewportPosition;
				USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), ParticlesInitData.AbsolutePosition, SegmentToSpawnFXPixelPosition, SegmentToSpawnFXViewportPosition);
				ParticleCanvasSlot->SetPosition(SegmentToSpawnFXViewportPosition);

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

void UUIVFXParticleManagerWidget::CheckIfShouldSpawnParticleAndSpawnIt()
{
	if (m_State.NumberOfParticlesSpawned == m_State.NumberOfParticlesToSpawn)
	{
		TimerManager->ClearTimer(CreateParticleTimerHandle);
		m_State.NumberOfParticlesSpawned = 0;
		m_State.NumberOfParticlesToSpawn = 0;
		return;
	}

	CreateParticle(m_State.InitData);
	m_State.NumberOfParticlesSpawned++;
}

#if WITH_EDITOR
void UUIVFXParticleManagerWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	MinNumberOfParticles = FMath::Clamp(MinNumberOfParticles, MinNumberOfParticles, MaxNumberOfParticles);
	MaxNumberOfParticles = FMath::Clamp(MaxNumberOfParticles, MinNumberOfParticles, MaxNumberOfParticles);
}
#endif
