// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceEntryWidget.h"

#include "ChoiceOptionConfig.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "MLlikeButton.h"
#include "MLlikeLogCategories.h"
#include "Components/Image.h"

void UChoiceEntryWidget::InitializeWithConfigAndDelayShowing(const UChoiceOptionConfig* const ChoiceConfig, float DelayInSecs)
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
	
	Button->OnClicked().AddUObject(this, &UChoiceEntryWidget::HandleButtonClicked);

	if (!IsDesignTime())
	{
		//BP_Show will show at the right time
		SetVisibility(ESlateVisibility::Hidden);
	
		GetWorld()->GetTimerManager().SetTimer(ShowTimerHandle, FTimerDelegate::CreateLambda([this]()
			{
				BP_Show();
			}),
			DelayInSecs,
			/*bInLoop*/ false
		);
	}

	BP_Initialize();
}

void UChoiceEntryWidget::HandleButtonClicked()
{
	OnChosen.ExecuteIfBound(Config);
}
