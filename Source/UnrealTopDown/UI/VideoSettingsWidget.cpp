// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/VideoSettingsWidget.h"
#include "UTDGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "UI/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoUserSetting, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    const auto* UserSettings = UUTDGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoUserSetting, Error, TEXT("UUTDGameUserSettings is null"))
        return;
    }

    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting: VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }
}
