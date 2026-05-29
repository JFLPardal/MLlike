// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MLlikeHUD.generated.h"

struct FUIVFXInitData;
class USegmentedBarWidget;
class UUIVFXParticleManagerWidget;

/**
 * 
 */
UCLASS()
class MLLIKE_API UMLlikeHUD : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USegmentedBarWidget> m_SegmentedBarWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UUIVFXParticleManagerWidget> m_ParticleManagerWidget = nullptr;

private:
	UFUNCTION()
	void OnPlaySegmentDepletedUIVFX(const FUIVFXInitData& InitData);
};
