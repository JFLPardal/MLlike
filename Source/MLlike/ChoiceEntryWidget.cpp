// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceEntryWidget.h"

#include "ChoiceOptionConfig.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "MLlikeLogCategories.h"
#include "Components/Image.h"

void UChoiceEntryWidget::InitializeWithConfig(const UChoiceOptionConfig* const ChoiceConfig)
{
	if (!IsValid(ChoiceConfig))
	{
		UE_LOG(LogMLlikeUI, Warning, TEXT("%s - Trying to Initialize ChoiceEntryWidget with invalid ChoiceOptionConfig"), TEXT(__FUNCSIG__));
		return;
	}

	Icon->SetBrushFromTexture(ChoiceConfig->Icon);
	Name->SetText(ChoiceConfig->Name);
	Description->SetText(ChoiceConfig->GetDescription());
	Rarity = ChoiceConfig->GetRarity();

	Config = ChoiceConfig;
	
	BP_Initialize();
}

void UChoiceEntryWidget::HandleButtonClicked()
{
	Super::HandleButtonClicked();

	OnChosen.ExecuteIfBound(Config);
}
