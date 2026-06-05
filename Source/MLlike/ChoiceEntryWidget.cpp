// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceEntryWidget.h"

#include "ChoiceOptionConfig.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"

void UChoiceEntryWidget::InitializeWithConfig(const UChoiceOptionConfig* const ChoiceConfig)
{
	Icon->SetBrushFromTexture(ChoiceConfig->Icon);
	Name->SetText(ChoiceConfig->Name);
	Description->SetText(ChoiceConfig->Description);
}
