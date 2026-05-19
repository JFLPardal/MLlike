// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDefinitionDataAsset.generated.h"

class UMaterialInstance;

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
	TObjectPtr<UMaterialInstance> Material;
};
