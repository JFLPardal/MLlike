// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "BarSegmentWidget.generated.h"

struct FUIVFXInitData;
class UWidgetAnimation;

/**
 * 
 */
UCLASS(MinimalAPI)
class UBarSegmentWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	void PopulateVFXInitData(FUIVFXInitData& InitData) const;

	void SetProgress(float NewProgress);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Setup)
	FLinearColor m_SegmentFullColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	FLinearColor m_SegmentNotFullColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	int32 m_MinNumberVFXParticles = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	int32 m_MaxNumberVFXParticles = 3;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BarFullAnimation;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnProgressChanged(float NewProgress, bool bWasBarFull, bool bIsBarFull);

private:
	float m_Progress = 1.0f;
};
