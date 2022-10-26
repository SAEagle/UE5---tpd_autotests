// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/VideoSettingsWidget.h"
#include "UTDGameUserSettings.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "UI/SettingOptionWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoUserSetting, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UUTDGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoUserSetting, Error, TEXT("UUTDGameUserSettings is null"))
        return;
    }

    UserSettings->LoadSettings();
    const auto VideoSettings = UserSettings->GetVideoSettings();

    check(VideoSettingsContainer);
    VideoSettingsContainer->ClearChildren();

    for (auto* Setting : VideoSettings)
    {
        const auto SettingWidget = CreateWidget<USettingOptionWidget>(this, SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton);
    RunBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnBenchmark);

    UserSettings->OnVideoSettingUpdated.AddUObject(this, &ThisClass::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto* UserSettings = UUTDGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!VideoSettingsContainer) return;
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingOptionWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingOptionWidget->UpdateTexts();
        }
    }
}
