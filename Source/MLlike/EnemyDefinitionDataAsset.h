// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "EnemyDefinitionDataAsset.generated.h"

class ATwinStickProjectile;
class UAnimMontage;
class UMaterialInstance;
class UStateTree;

/** struct to initialize MeleeAttackComponent */
USTRUCT(BlueprintType)
struct FMeleeAttackComponentConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToTrace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MakeStructureDefaultValue="False"))
	bool HasWindupAnim;
};

/** struct to initialize ShootingAttackComponent */
USTRUCT(BlueprintType)
struct FShootingAttackComponentConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> AimingAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> ShootingAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ATwinStickProjectile> ProjectileClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ProjectileSpawnSocketName;
};

/**
 * 
 */
UCLASS()
class MLLIKE_API UEnemyDefinitionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	float GetMaxInitialHealth() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStateTree> StateTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 1, ClampMax = 1000))
	float MaxInitialHealth = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behaviours)
	bool bHasMeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behaviours, meta=(EditCondition="bHasMeleeAttack", EditConditionHides))
	FMeleeAttackComponentConfig MeleeAttackComponentConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behaviours)
	bool bHasShootingAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behaviours, meta = (EditCondition = "bHasShootingAttack", EditConditionHides))
	FShootingAttackComponentConfig ShootingAttackComponentConfig;
};
