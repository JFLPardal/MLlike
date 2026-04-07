// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BarSegmentWidget.generated.h"

class UWidgetAnimation;

UCLASS(MinimalAPI)
class UBarSegmentData : public UObject
{
	GENERATED_BODY()

	DECLARE_DELEGATE_TwoParams(FOnBarSegmentDataChanged, float /*OldProgress*/, float /*NewProgress*/);

public:
	FOnBarSegmentDataChanged OnBarSegmentDataChanged;

	void SetBarSegmentProgress(float NewProgress);
	float GetBarSegmentProgress() const { return m_Progress; }

private:
	float m_Progress = 0.0f;
};

/**
 * 
 */
UCLASS(MinimalAPI)
class UBarSegmentWidget : public UMLlikeWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeDestruct() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_SegmentFullColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor m_SegmentNotFullColor;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BarFullAnimation;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnProgressChanged(float NewProgress, bool bWasBarFull, bool bIsBarFull);

private:
	void OnBarSegmentDataChanged(float OldProgress, float NewProgress);
};
