// Fill out your copyright notice in the Description page of Project Settings.


#include "MLlikeNPC.h"

#include "BaseHealthAttributeSet.h"
#include "MLLikeAbilitySystemComponent.h"

AMLlikeNPC::AMLlikeNPC()
{
	m_ASC = CreateDefaultSubobject<UMLLikeAbilitySystemComponent>(TEXT("ASC"));

	UBaseHealthAttributeSet* BaseHealthAttributeSet = NewObject<UBaseHealthAttributeSet>(this, TEXT("BaseHealthAttributeSet"));
	m_ASC->AddAttributeSetSubobject<UBaseHealthAttributeSet>(BaseHealthAttributeSet);
}

void AMLlikeNPC::BeginPlay()
{
	Super::BeginPlay();

	m_ASC->InitAbilityActorInfo(this, this);
}
