// Fill out your copyright notice in the Description page of Project Settings.


#include "MLlikeHUD.h"

#include "SegmentedBarWidget.h"
#include "UIVFXParticleManagerWidget.h"

void UMLlikeHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_SegmentedBarWidget->OnPlayUIVFX.BindDynamic(this, &UMLlikeHUD::OnPlaySegmentDepletedUIVFX);
}

void UMLlikeHUD::OnPlaySegmentDepletedUIVFX(const FUIVFXInitData& InitData)
{
	m_ParticleManagerWidget->PlayFromPosition(FVector2D{ InitData.Position.X, InitData.Position.Y });
}
