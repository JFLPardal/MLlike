// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickAIController.h"

#include "Components/StateTreeAIComponent.h"
#include "MLlikeLogCategories.h"
#include "StateTree.h"
#include "TwinStickNPC.h"

ATwinStickAIController::ATwinStickAIController()
{
	// create the StateTree AI Component
	StateTreeAI = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeAI"));
	check(StateTreeAI);

	bStartAILogicOnPossess = false;

	if (IsValid(StateTreeAI))
	{
		StateTreeAI->SetStartLogicAutomatically(false);
	}

	// ensure we're attached to the possessed character.
	// this is necessary for EnvQueries to work correctly
	bAttachToPawn = true;
}

void ATwinStickAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATwinStickNPC* const TwinStickNPC = Cast<ATwinStickNPC>(InPawn); IsValid(TwinStickNPC))
	{
		SetStateTree(TwinStickNPC->GetStateTree());
	}
	else
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Couldn't cast InPawn %s to ATwinStickNPC. Can't set state tree for enemy"), TEXT(__FUNCSIG__), *InPawn->GetHumanReadableName());
	}
}

void ATwinStickAIController::SetStateTree(UStateTree* const StateTree)
{
	if (!IsValid(StateTree))
	{
		UE_LOG(LogMLlikeGeneral, Warning, TEXT("Calling %s with an invalid StateTree for %s"), TEXT(__FUNCSIG__), *GetPawn()->GetHumanReadableName());
		return;
	}

	if (!IsValid(StateTreeAI))
	{
		return;
	}

	StateTreeAI->SetStateTree(StateTree);
	StateTreeAI->StartLogic();
}
