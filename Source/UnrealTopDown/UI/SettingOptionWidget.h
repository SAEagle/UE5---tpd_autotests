// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingOptionWidget.generated.h"

class UTextBlock;
class UTDGameSetting;
class UButton;

UCLASS()
class UNREALTOPDOWN_API USettingOptionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingDisplayName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingCurrentValue;

    UPROPERTY(meta = (BindWidget))
    UButton* NextSettingButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PrevSettingButton;

    virtual void NativeOnInitialized() override;

private:
    TWeakObjectPtr<UTDGameSetting> Setting;

    void Init(UTDGameSetting* InSetting);
    void UpdateTexts();

    UFUNCTION()
    void OnNextSetting();

    UFUNCTION()
    void OnPrevSetting();

    friend class UVideoSettingsWidget;
};
