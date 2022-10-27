// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UTDGameUserSettings.h"
#include "UI/UTDGameSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 Level)             \
    {                            \
        FUNC(Level);             \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "GameUserSettings"

UUTDGameUserSettings::UUTDGameUserSettings()
{
    const TArray<FSettingOption> VideoSettingsOptions = {
        {LOCTEXT("VFXQualityLow_Loc", "Low"), 0},        //
        {LOCTEXT("VFXQualityMedium_Loc", "Medium"), 1},  //
        {LOCTEXT("VFXQualityHigh_Loc", "High"), 2},      //
        {LOCTEXT("VFXQualityEpic_Loc", "Epic"), 3}       //
    };

    {
        auto* Setting = NewObject<UTDGameSetting>();
        check(Setting) Setting->SetName(LOCTEXT("AntiAliasing", "Anti-Aliasing"));
        Setting->SetOption(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName(LOCTEXT("Textures", "Textures"));
        Setting->SetOption(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName(LOCTEXT("GlobalIlum", "Global Illumination"));
        Setting->SetOption(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetGlobalIlluminationQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetGlobalIlluminationQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName(LOCTEXT("Shadows", "Shadows"));
        Setting->SetOption(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetShadowQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadowQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<UTDGameSetting>();
        Setting->SetName(LOCTEXT("PostProcessing", "Post Processing"));
        Setting->SetOption(VideoSettingsOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
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

void UUTDGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnVideoSettingUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE