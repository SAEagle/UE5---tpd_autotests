// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingOptionWidget.h"
#include "UTDGameSetting.h"
#include "Components/TextBlock.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    check(SettingDisplayName);
}

void USettingOptionWidget::Init(UTDGameSetting* InSetting)
{
    Setting = MakeWeakObjectPtr(InSetting);

    UpdateTexts();
}

void USettingOptionWidget::UpdateTexts()
{
    if (Setting.IsValid())
    {
        SettingDisplayName->SetText(FText::FromString(Setting->GetName()));
        SettingCurrentValue->SetText(FText::FromString(Setting->GetCurrentOption().Name));
    }
}
