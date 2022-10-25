// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UTDGameSetting.generated.h"

USTRUCT()
struct FSettingOption
{
    GENERATED_BODY()

    FString Name;

    int32 Value;
};

UCLASS()
class UTDGameSetting : public UObject
{
    GENERATED_BODY()
public:
    void SetName(const FString& InName);
    void SetOption(const TArray<FSettingOption>& InOptions);

    FSettingOption GetCurrentOption() const;
    FString GetName() const;

private:
    FString Name;
    TArray<FSettingOption> Options;
};