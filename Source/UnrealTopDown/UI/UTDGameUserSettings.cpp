// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UTDGameUserSettings.h"
#include "UI/UTDGameSetting.h"

UUTDGameUserSettings::UUTDGameUserSettings()
{
    const TArray<FSettingOption> VFXOptions = {{"Low", 0}, {"Medium", 1}, {"High", 2}, {"Epic", 3}};
    {
        auto* Setting = NewObject<UTDGameSetting>();
        check(Setting)
        Setting->SetName("Anti-Aliasing");
        Setting->SetOption(VFXOptions);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName("Textures");
        Setting->SetOption(VFXOptions);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName("Global Ilumination");
        Setting->SetOption(VFXOptions);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName("Shadows");
        Setting->SetOption(VFXOptions);
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName("Post Processing");
        Setting->SetOption(VFXOptions);
        VideoSettings.Add(Setting);
    }
}

UUTDGameUserSettings* UUTDGameUserSettings::Get()
{
    return GEngine ? Cast<UUTDGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<UTDGameSetting*>& UUTDGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}
