// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "MLlikeWidget.h"

#include "SegmentedBarWidget.generated.h"

struct FEnergyAmountChangedData;
struct FMaxAmmoChangedData;
class UListView;
class UPanelWidget;

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
	void OnCurrentEnergyAmountChanged(FGameplayTag Channel, const FEnergyAmountChangedData& EnergyAmountChangedData);

	UFUNCTION()
	void OnMaxAmmoChanged(FGameplayTag Channel, const FMaxAmmoChangedData& EnergyAmountChangedData);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UListView> SegmentsList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UPanelWidget> SegmentsBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMLlikeWidget> SegmentWidgetClass;

private:
	FGameplayMessageListenerHandle EnergyChangedHandle;
	FGameplayMessageListenerHandle MaxAmmoChangedHandle;
};
