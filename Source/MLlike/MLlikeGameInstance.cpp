// Fill out your copyright notice in the Description page of Project Settings.


#include "MLlikeGameInstance.h"

#include "CoreGlobals.h"
#include "HAL/IConsoleManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"

void UMLlikeGameInstance::Init()
{
	Super::Init();
#if !UE_BUILD_SHIPPING
	ApplyLocalDevSettings();
#endif
}

#if !UE_BUILD_SHIPPING
void UMLlikeGameInstance::ApplyLocalDevSettings()
{
	const FString DevSettingsPath = FPaths::ProjectConfigDir() / TEXT("DefaultLocalDev.ini");
	TArray<FString> CVarOverrides;

	GConfig->UnloadFile(DevSettingsPath);
	GConfig->LoadFile(DevSettingsPath);

	GConfig->GetSection(
		TEXT("ConsoleVariables"),
		CVarOverrides,
		DevSettingsPath
	);

	for (const FString& CVarOverride : CVarOverrides)
	{
		FString Name;
		FString Value;

		if (CVarOverride.Split(TEXT("="), &Name, &Value))
		{
			Name.TrimStartAndEndInline();
			Value.TrimStartAndEndInline();

			if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*Name))
			{
				CVar->Set(*Value);

				UE_LOG(LogTemp, Log,
					TEXT("Applied local cvar: %s=%s"),
					*Name,
					*Value);
			}
		}
	}
}
#endif