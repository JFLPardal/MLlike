// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableHUD.h"

#include "HealthBarInitData.h"

void UDamageableHUD::Init(const FDamageableHUDInitData& InitData)
{
	HealthBar->Init(InitData.HealthBarInitData);
}
