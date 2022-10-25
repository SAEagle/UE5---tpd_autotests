// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UTDGameUserSettings.generated.h"

class UTDGameSetting;

UCLASS()
class UNREALTOPDOWN_API UUTDGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    UUTDGameUserSettings();
    static UUTDGameUserSettings* Get();

    const TArray<UTDGameSetting*>& GetVideoSettings() const;

private:
    UPROPERTY()
    TArray<UTDGameSetting*> VideoSettings;
};
