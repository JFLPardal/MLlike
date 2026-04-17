// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickNPC.h"

#include "BaseHealthAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MLLikeAbilitySystemComponent.h"
#include "TimerManager.h"
#include "TwinStickCharacter.h"
#include "TwinStickGameMode.h"
#include "TwinStickNPCDestruction.h"
#include "TwinStickPickup.h"

ATwinStickNPC::ATwinStickNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	// ensure we spawn an AI controller when we're spawned
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// configure the inherited components
	GetCapsuleComponent()->SetCapsuleRadius(45.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFriction = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 250.0f;
	GetCharacterMovement()->AvoidanceWeight = 1.0f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	m_ASC = CreateDefaultSubobject<UMLLikeAbilitySystemComponent>(TEXT("ASC"));

	UBaseHealthAttributeSet* BaseHealthAttributeSet = NewObject<UBaseHealthAttributeSet>(this, TEXT("BaseHealthAttributeSet"));
	m_ASC->AddAttributeSetSubobject<UBaseHealthAttributeSet>(BaseHealthAttributeSet);

	m_ASC->GetGameplayAttributeValueChangeDelegate(BaseHealthAttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &ATwinStickNPC::OnCurrentHealthChanged);
}

void ATwinStickNPC::BeginPlay()
{
	Super::BeginPlay();

	// increment the NPC counter so we can cap spawning if necessary
	if (ATwinStickGameMode* GM = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->IncreaseNPCs();
	}

	m_ASC->InitAbilityActorInfo(this, this);
}

void ATwinStickNPC::OnCurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		Killed();
	}
}

void ATwinStickNPC::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the destruction timer
	GetWorld()->GetTimerManager().ClearTimer(DestructionTimer);
}

void ATwinStickNPC::Destroyed()
{
	// decrease the NPC counter so we can cap spawning if necessary
	if (ATwinStickGameMode* GM = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->DecreaseNPCs();
	}

	Super::Destroyed();
}

void ATwinStickNPC::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// have we collided against the player?
	if (ATwinStickCharacter* PlayerCharacter = Cast<ATwinStickCharacter>(Other))
	{
		// apply damage to the character
		PlayerCharacter->HandleDamage(1.0f, GetActorForwardVector());
	}
}

void ATwinStickNPC::Killed()
{
	// only handle damage if we haven't been hit yet
	if (bHit)
	{
		return;
	}

	// raise the hit flag
	bHit = true;

	// deactivate character movement
	GetCharacterMovement()->Deactivate();

	// award points
	if (ATwinStickGameMode* GM = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->ScoreUpdate(Score);
	}

	// randomly spawn a pickup
	if (FMath::RandRange(0, 100) < PickupSpawnChance)
	{
		ATwinStickPickup* Pickup = GetWorld()->SpawnActor<ATwinStickPickup>(PickupClass, GetActorTransform());
	}
	
	// spawn the NPC destruction proxy
	ATwinStickNPCDestruction* DestructionProxy = GetWorld()->SpawnActor<ATwinStickNPCDestruction>(DestructionProxyClass, GetActorTransform());

	// hide this actor
	SetActorHiddenInGame(true);

	// disable collision
	SetActorEnableCollision(false);

	// defer destruction
	GetWorld()->GetTimerManager().SetTimer(DestructionTimer, this, &ATwinStickNPC::DeferredDestroy, DeferredDestructionTime, false);
}

void ATwinStickNPC::DeferredDestroy()
{
	// destroy this actor
	Destroy();
}
