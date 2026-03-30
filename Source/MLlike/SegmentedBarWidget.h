// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AmmoAmountChangedData.h"
#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "MLlikeWidget.h"

#include "SegmentedBarWidget.generated.h"

class UListView;

/**
 * 
 */
UCLASS(MinimalAPI)
class USegmentedBarWidget : public UMLlikeWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	// this is too specific for the ammo, should not be in USegmentedBarWidget that intends to be more generic
	UFUNCTION()
	void OnCurrentAmmoAmountChanged(FGameplayTag Channel, const FAmmoAmountChangedData& AmmoAmountChangedData);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UListView> SegmentsList;

private:
	FGameplayMessageListenerHandle AmmoChangedHandle;
};
