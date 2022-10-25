// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UTDGameSetting.h"

void UTDGameSetting::SetName(const FString& InName)
{
    Name = InName;
}

void UTDGameSetting::SetOption(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UTDGameSetting::GetCurrentOption() const
{
    return Options[0];
}

FString UTDGameSetting::GetName() const
{
    return Name;
}
