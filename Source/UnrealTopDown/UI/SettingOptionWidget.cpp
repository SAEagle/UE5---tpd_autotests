// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingOptionWidget.h"
#include "UTDGameSetting.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    check(SettingDisplayName);
    check(NextSettingButton);
    check(PrevSettingButton);

    NextSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnNextSetting);
    PrevSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevSetting);
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
        SettingDisplayName->SetText(Setting->GetName());
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnNextSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyNextOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}

void USettingOptionWidget::OnPrevSetting()
{
    if (Setting.IsValid())
    {
        Setting->ApplyPrevOption();
        SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
    }
}
