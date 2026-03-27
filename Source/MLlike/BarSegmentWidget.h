// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BarSegmentWidget.generated.h"


UCLASS(MinimalAPI)
class UBarSegmentData : public UObject
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnBarSegmentDataChanged, bool /*bIsBarSegmentActive*/);

public:
	FOnBarSegmentDataChanged OnBarSegmentDataChanged;

	void SetIsBarSegmentActive(const bool bIsActive);
	bool GetIsBarSegmentActive() const { return bIsBarSegmentActive; }

private:
	bool bIsBarSegmentActive = true;
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
	TObjectPtr<UWidgetAnimation> HideAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBarSegmentActive;

private:
	void OnBarSegmentDataChanged(bool bBarSegmentActive);
};
