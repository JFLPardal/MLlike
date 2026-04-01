// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BarSegmentWidget.generated.h"


class UProgressBar;

UCLASS(MinimalAPI)
class UBarSegmentData : public UObject
{
	GENERATED_BODY()

	DECLARE_DELEGATE_TwoParams(FOnBarSegmentDataChanged, float /*OldProgress*/, float /*NewProgress*/);

public:
	FOnBarSegmentDataChanged OnBarSegmentDataChanged;

	void SetBarSegmentProgress(float NewProgress);

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
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowHideAnimation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> SegmentFullAnimation;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> m_ProgressBar;

private:
	void OnBarSegmentDataChanged(float OldProgress, float NewProgress);
};
