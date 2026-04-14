// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "BarSegmentWidget.generated.h"

struct FUIVFXInitData;
class UWidgetAnimation;

// in ML, the left edge segment has its left corners less round and the right edge segment has its right corners less round. This enum informs the segment if it's one of those ( if there's only one segment, segment is both edges )
UENUM(BlueprintType)
enum class EBarSegmentEdgeDescription : uint8
{
	None,
	Left,
	Right,
	Both
};

USTRUCT()
struct FBarSegmentInitData
{
	GENERATED_BODY()

	FMargin Padding;

	EBarSegmentEdgeDescription EdgeDescription = EBarSegmentEdgeDescription::None;
};
/**
 * 
 */
UCLASS(MinimalAPI)
class UBarSegmentWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	void Init(const FBarSegmentInitData& InitData);

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

	UFUNCTION(BlueprintImplementableEvent)
	void BP_UpdateEdgeDescription(EBarSegmentEdgeDescription Description);

private:
	float m_Progress = 1.0f;
};
