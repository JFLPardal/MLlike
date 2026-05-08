// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TwinStickAIController.generated.h"

class UStateTreeAIComponent;
class UStateTree;

/**
 *  A StateTree-Enabled AI Controller for a Twin Stick Shooter game
 *  Runs NPC logic through a StateTree
 */
UCLASS(abstract)
class ATwinStickAIController : public AAIController
{
	GENERATED_BODY()
	
	/** StateTree Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStateTreeAIComponent* StateTreeAI;

public:

	/** Constructor */
	ATwinStickAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void SetStateTree(UStateTree* const StateTree);
};
