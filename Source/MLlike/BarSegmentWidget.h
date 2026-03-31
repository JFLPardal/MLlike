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

	DECLARE_DELEGATE_ThreeParams(FOnBarSegmentDataChanged, bool /*bIsBarSegmentActive*/, float /*OldProgress*/, float /*NewProgress*/);

public:
	FOnBarSegmentDataChanged OnBarSegmentDataChanged;
 
	void SetIsBarSegmentActive(const bool bIsActive);
	bool GetIsBarSegmentActive() const { return m_bIsBarSegmentActive; }
	void ToggleIsBarSegmentActive();

	void SetBarSegmentProgress(float NewProgress);

private:
	bool m_bIsBarSegmentActive = true;
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

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> m_ProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBarSegmentActive;

private:
	void OnBarSegmentDataChanged(bool bBarSegmentActive, float OldProgress, float NewProgress);
};
