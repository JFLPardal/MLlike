// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickNPC.h"

#include "BaseHealthAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyDefinitionDataAsset.h"
#include "EnemySpawningSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "HealthBarWidget.h"
#include "MLLikeAbilitySystemComponent.h"
#include "MLlikeGameplayTags.h"
#include "TimerManager.h"
#include "TwinStickCharacter.h"
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

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidgetComponent->SetupAttachment(GetMesh());

	m_ASC = CreateDefaultSubobject<UMLLikeAbilitySystemComponent>(TEXT("ASC"));

	m_HealthAttributeSet = CreateDefaultSubobject<UBaseHealthAttributeSet>(TEXT("BaseHealthAttributeSet"));
}

UAbilitySystemComponent* ATwinStickNPC::GetAbilitySystemComponent() const
{
	return m_ASC;
}

UStateTree* const ATwinStickNPC::GetStateTree() const
{
	return StateTree;
}

void ATwinStickNPC::SetEnemyDefinitionDataAsset(UEnemyDefinitionDataAsset* const DataAsset)
{
	m_DefinitionAsset = DataAsset;
}

void ATwinStickNPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_ASC))
	{
		m_ASC->InitAbilityActorInfo(this, this);
	
		FGameplayEffectSpecHandle SpecHandle = m_ASC->MakeOutgoingSpec(m_HealthAttributeSetInitGE, 1.0f, m_ASC->MakeEffectContext());
		SpecHandle.Data->SetSetByCallerMagnitude(MLlikeGameplayTags::TAG_MLlike_Attribute_BaseHealth_MaxHealth, (IsValid(m_DefinitionAsset)) ? m_DefinitionAsset->GetMaxInitialHealth() : 1);
		m_ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

		if(IsValid(m_HealthAttributeSet))
		{
			m_HealthAttributeSet->InitDependentAttributes();
		}

		m_ASC->GetGameplayAttributeValueChangeDelegate(m_HealthAttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &ATwinStickNPC::OnCurrentHealthChanged);

		if (IsValid(HealthBarWidgetComponent))
		{
			HealthBarWidgetComponent->InitWidget();
			if (UHealthBarWidget* Widget = Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetWidget()); IsValid(Widget))
			{
				if(IsValid(m_HealthAttributeSet))
				{
					FHealthBarInitData InitData;
					InitData.m_ASC = m_ASC;
					InitData.m_CurrentHealthAttribute = m_HealthAttributeSet->GetCurrentHealthAttribute();
					InitData.m_MaxHealth = m_HealthAttributeSet->GetMaxHealth();
					Widget->Init(InitData);
				}
			}

			HealthBarWidgetComponent->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
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
	if (UEnemySpawningSubsystem* const EnemySpawningSubsystem = GetWorld()->GetSubsystem<UEnemySpawningSubsystem>(); IsValid(EnemySpawningSubsystem))
	{
		EnemySpawningSubsystem->EnemyDestroyed();
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
