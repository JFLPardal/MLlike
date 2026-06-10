// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoiceScreenWidget.h"

#include "ChoiceEntryWidget.h"
#include "CommonButtonBase.h"

void UChoiceScreenWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
	{
		if (IsValid(DesignerTestingConfig.ChoiceEntryWidgetClass) && !DesignerTestingConfig.ChoicesToShow.IsEmpty())
		{
			Cleanup();
			InitializeWithConfig(DesignerTestingConfig);
			bChoicesInitialized = false;
		}
	}
}

void UChoiceScreenWidget::InitializeWithConfig(const FChoiceScreenWidgetConfig& Config)
{
	ensureMsgf(!bChoicesInitialized, TEXT("%s - Trying to Initialize a ChoiceScreenWidget that is already Initialized"), TEXT(__FUNCSIG__));
	if (!IsValid(*Config.ChoiceEntryWidgetClass))
	{
		ensureMsgf(false, TEXT("ChoiceEntryWidgetClass is invalid - won't show ChoiceScreenWidget"));
		DeactivateWidget();
		return;
	}

	for (UChoiceOptionConfig const* const ChoiceOptionConfig : Config.ChoicesToShow)
	{
		if (IsValid(ChoiceOptionConfig))
		{
			UChoiceEntryWidget* const ChoiceEntryWidget = CreateWidget<UChoiceEntryWidget>(this, Config.ChoiceEntryWidgetClass);
			ChoiceEntryWidget->SetPadding(PaddingForEntries);
			ChoiceEntryWidget->InitializeWithConfig(ChoiceOptionConfig);
			ChoiceEntryWidget->OnChosen.BindUObject(this, &UChoiceScreenWidget::HandleOnChoiceMade);

			m_ChoicesList->AddChild(ChoiceEntryWidget);
		}
	}

	bChoicesInitialized = true;
}

void UChoiceScreenWidget::HandleOnChoiceMade(const UChoiceOptionConfig* const ChosenConfig)
{
	OnChoiceMade.ExecuteIfBound(ChosenConfig);

	DeactivateWidget();
}

void UChoiceScreenWidget::Cleanup()
{
	OnChoiceMade.Unbind();
	bChoicesInitialized = false;
	m_ChoicesList->ClearChildren();
}

void UChoiceScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	Cleanup();
}

FChoiceScreenWidgetConfig::FChoiceScreenWidgetConfig(TSubclassOf<UChoiceEntryWidget> InChoiceEntryWidgetClass, TArray<const UChoiceOptionConfig* const>& InChoicesToShow)
	: ChoiceEntryWidgetClass(InChoiceEntryWidgetClass)
	, ChoicesToShow(InChoicesToShow)
{
}

FChoiceScreenWidgetConfig::FChoiceScreenWidgetConfig()
	:ChoiceEntryWidgetClass(nullptr)
{
}
