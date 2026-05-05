// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwinStickGameMode.generated.h"

class UMLlikeHUD;

/**
 *  Simple Game Mode for a Twin Stick Shooter game.
 */
UCLASS(abstract)
class ATwinStickGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	/** Type of UI Widget to spawn */
	UPROPERTY(EditAnywhere, Category="Twin Stick")
	TSubclassOf<UMLlikeHUD> UIWidgetClass;

	/** Pointer to the spawned UI Widget */
	TObjectPtr<UMLlikeHUD> UIWidget;

	/** Max number of NPCs to allow in the level at once */
	UPROPERTY(EditAnywhere, Category="Twin Stick", meta=(ClampMin = 0, ClampMax = 100))
	int32 NPCCap = 20;

	/** Current number of NPCs in the level */
	int32 NPCCount = 0;

public:

	/** Gameplay initialization */
	virtual void BeginPlay() override;

public:

	/** Returns true if the number of NPCs is under the cap */
	bool CanSpawnNPCs();

	/** Increases the NPC count */
	void IncreaseNPCs();

	/** Decreases the NPC count */
	void DecreaseNPCs();
};
