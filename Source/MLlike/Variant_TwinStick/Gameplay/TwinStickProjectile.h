// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TwinStickProjectile.generated.h"

class UMLLikeAbilitySystemComponent;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
/**
 *  A simple bouncing projectile for a Twin Stick shooter game
 */
UCLASS(abstract)
class ATwinStickProjectile : public AActor
{
	GENERATED_BODY()
	
	/** Projectile collision sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	/** Mesh that provides the visual representation for this projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	/** Handles movement behaviors for this projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:	

	/** Constructor */
	ATwinStickProjectile();

	UFUNCTION(BlueprintCallable)
	void SetOwnerASC(UMLLikeAbilitySystemComponent* OwnerASC);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMLLikeAbilitySystemComponent> m_OwnerASC;

protected:
	/** Handles collisions that stop this projectile from moving */
	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

};
