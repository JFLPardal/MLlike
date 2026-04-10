// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MLlikeWidget.h"
#include "UIVFXParticleManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MLLIKE_API UUIVFXParticleManagerWidget : public UMLlikeWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayFromPosition(FVector2D InitialPosition);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// TODO restrict this further?
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMLlikeWidget> ParticleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin=2,UIMax=15, ClampMin=2, ClampMax=15))
	int32 MinNumberOfParticles = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 2, UIMax = 15, ClampMin = 2, ClampMax = 15))
	int32 MaxNumberOfParticles = 4;

};
