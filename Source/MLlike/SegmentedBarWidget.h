// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Layout/Margin.h"
#include "MLlikeWidget.h"

#include "SegmentedBarWidget.generated.h"

struct FEnergyAmountChangedData;
struct FMaxAmmoChangedData;
struct FUIVFXInitData;
class UBarSegmentWidget;
class UHorizontalBox;

DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayUIVFX, const FUIVFXInitData&, InitData);

/**
 * 
 */
UCLASS(MinimalAPI)
class USegmentedBarWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	FPlayUIVFX OnPlayUIVFX;

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
	TObjectPtr<UHorizontalBox> SegmentsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBarSegmentWidget> SegmentClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMargin PaddingPerSegment;

private:
	FGameplayMessageListenerHandle EnergyChangedHandle;
	FGameplayMessageListenerHandle MaxAmmoChangedHandle;
};
