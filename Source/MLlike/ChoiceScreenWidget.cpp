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

void UChoiceScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	m_ConfirmButton->OnClicked().AddUObject(this, &UChoiceScreenWidget::HandleOnConfirmButtonClicked);
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

			m_ChoicesList->AddChild(ChoiceEntryWidget);
		}
	}

	bChoicesInitialized = true;
}

void UChoiceScreenWidget::HandleOnConfirmButtonClicked()
{
	OnChoiceMade.ExecuteIfBound();

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

FChoiceScreenWidgetConfig::FChoiceScreenWidgetConfig(TArray<const UChoiceOptionConfig* const>& InChoicesToShow, TSubclassOf<UChoiceEntryWidget> InChoiceEntryWidgetClass)
	: ChoicesToShow(InChoicesToShow)
	, ChoiceEntryWidgetClass(InChoiceEntryWidgetClass)
{
}

FChoiceScreenWidgetConfig::FChoiceScreenWidgetConfig()
	:ChoiceEntryWidgetClass(nullptr)
{
}
