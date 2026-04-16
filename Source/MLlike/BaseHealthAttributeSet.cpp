// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthAttributeSet.h"

UBaseHealthAttributeSet::UBaseHealthAttributeSet()
{
	InitMaxHealth(1);
	InitCurrentHealth(GetMaxHealth());
}
