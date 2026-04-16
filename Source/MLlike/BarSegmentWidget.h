// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
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

USTRUCT(BlueprintType)
struct FBarSegmentInitData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FMargin Padding;

	UPROPERTY(BlueprintReadOnly)
	EBarSegmentEdgeDescription EdgeDescription = EBarSegmentEdgeDescription::None;

	UPROPERTY(BlueprintReadOnly)
	FLinearColor Color;
	
	UPROPERTY(BlueprintReadOnly)
	FLinearColor ColorIfBarIsFull;
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

	void UpdateFullColor(FLinearColor NewColor);

	void BarFullStateChanged(bool bIsBarFull);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	FLinearColor m_FillColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	int32 m_MinNumberVFXParticles = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Setup)
	int32 m_MaxNumberVFXParticles = 3;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SegmentFullAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BarFullAnimation;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Init(const FBarSegmentInitData& InitData);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnProgressChanged(float NewProgress, bool bWasSegmentFull, bool bIsSegmentFull);
		
	UFUNCTION(BlueprintImplementableEvent)
	void BP_UpdateSegmentFullColor(FLinearColor NewColor);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_BarFullStateChanged(bool bIsBarFull);
	
	UFUNCTION(BlueprintCallable)
	bool IsSegmentFull() const;

private:
	float m_Progress = 1.0f;
};
