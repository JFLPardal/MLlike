// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDefinitionDataAsset.generated.h"

class ATwinStickProjectile;
class UAnimMontage;
class UMaterialInstance;
class UStateTree;

//TODO move to individual file, add include to MeleeAttackComponent
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

//TODO move to individual file, add include to MeleeAttackComponent
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasMeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bHasMeleeAttack", EditConditionHides))
	FMeleeAttackComponentConfig MeleeAttackComponentConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHasShootingAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bHasShootingAttack", EditConditionHides))
	FShootingAttackComponentConfig ShootingAttackComponentConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStateTree> StateTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> Material;
};
