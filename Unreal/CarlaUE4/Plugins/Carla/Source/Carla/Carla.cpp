// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Carla.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Developer/Settings/Public/ISettingsContainer.h"

#define LOCTEXT_NAMESPACE "FCarlaModule"

DEFINE_LOG_CATEGORY(LogCarla);
DEFINE_LOG_CATEGORY(LogCarlaServer);

void FCarlaModule::StartupModule()
{
	RegisterSettings();
}

void FCarlaModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

void FCarlaModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, add here all those settings you want to expose
	// to your LDs or artists.

	// if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	// {
	// 	// Create the new category
	// 	ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

	// 	SettingsContainer->DescribeCategory("CARLASettings",
	// 		LOCTEXT("RuntimeWDCategoryName", "CARLA Settings"),
	// 		LOCTEXT("RuntimeWDCategoryDescription", "CARLA plugin settings"));

	// 	// Register the settings
	// 	ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "CARLASettings", "General",
	// 		LOCTEXT("RuntimeGeneralSettingsName", "General"),
	// 		LOCTEXT("RuntimeGeneralSettingsDescription", "General configuration for the CARLA plugin"),
	// 		GetMutableDefault<UCarlaSettings>()
	// 	);

	// 	// Register the save handler to your settings, you might want to use it to
	// 	// validate those or just act to settings changes.
	// 	if (SettingsSection.IsValid())
	// 	{
	// 		SettingsSection->OnModified().BindRaw(this, &FCarlaModule::HandleSettingsSaved);
	// 	}
	// }
}

void FCarlaModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	// if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	// {
	// 	SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
	// }
}

bool FCarlaModule::HandleSettingsSaved()
{
	// UCarlaSettings* Settings = GetMutableDefault<UCarlaSettings>();
	// bool ResaveSettings = false;

	// // Put any validation code in here and resave the settings in case an invalid
	// // value has been entered

	// if (ResaveSettings)
	// {
	// 	Settings->SaveConfig();
	// }

	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCarlaModule, Carla)
