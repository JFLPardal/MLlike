// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwinStickGameMode.generated.h"


class UUIRootWidget;

/**
 *  Simple Game Mode for a Twin Stick Shooter game.
 */
UCLASS(abstract)
class ATwinStickGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	/** Type of UI Widget to spawn */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIRootWidget> UIWidgetClass;
};
