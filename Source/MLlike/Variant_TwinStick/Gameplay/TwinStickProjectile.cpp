// Copyright Epic Games, Inc. All Rights Reserved.


#include "TwinStickProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MLLikeAbilitySystemComponent.h"
#include "MLlikeLogCategories.h"
#include "TwinStickNPC.h"

ATwinStickProjectile::ATwinStickProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;

	// this actor will be destroyed automatically once InitialLifeSpan expires
	InitialLifeSpan = 2.0f;

	// create the collision sphere and set it as the root component
	RootComponent = CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	CollisionSphere->SetSphereRadius(35.0f);
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);

	// create the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionProfileName(FName("NoCollision"));

	// create the projectile movement comp. No need to attach it because it's not a scene component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 15000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bRotationRemainsVertical = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bForceSubStepping = true;

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ATwinStickProjectile::OnProjectileStop);
}

void ATwinStickProjectile::SetOwnerASC(UMLLikeAbilitySystemComponent* OwnerASC)
{
	if (!IsValid(OwnerASC))
	{
		UE_LOG(LogMLlikeGeneral, Error, TEXT("%s - Trying to set OwnerASC for %s with invalid pointer. This projectile will not behave as expected"), TEXT(__FUNCSIG__), *GetName());
		return;
	}

	m_OwnerASC = OwnerASC;
}

void ATwinStickProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	// destroy this actor immediately
	Destroy();
}
