// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UTDGameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameSetting, All, All);

void UTDGameSetting::SetName(const FText& InName)
{
    Name = InName;
}

void UTDGameSetting::SetOption(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UTDGameSetting::GetCurrentOption() const
{
    const int32 CurrentValue = GetCurrentValue();
    const auto Option = Options.FindByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
    if (!Option)
    {
        UE_LOG(LogGameSetting, Error, TEXT("Option does not exist"));
        return FSettingOption{};
    }
    return *Option;
}

FText UTDGameSetting::GetName() const
{
    return Name;
}

void UTDGameSetting::AddGetter(TFunction<int32()> Func)
{
    Getter = Func;
}

int32 UTDGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogGameSetting, Error, TEXT("Getter func is not set for %s"), *Name.ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UTDGameSetting::SetCurrentValue(int32 Value)
{
    if (!Setter)
    {
        UE_LOG(LogGameSetting, Error, TEXT("Setter func is not set for %s"), *Name.ToString());
        return;
    }
    Setter(Value);
}
int32 UTDGameSetting::GetCurrentIndex() const {
    const int32 CurrentValue = GetCurrentValue();
    return Options.IndexOfByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
    return int32();
}

void UTDGameSetting::AddSetter(TFunction<void(int32)> Func)
{
    Setter = Func;
}

void UTDGameSetting::ApplyNextOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
    SetCurrentValue(Options[NextIndex].Value);
}
void UTDGameSetting::ApplyPrevOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 PrevIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(Options[PrevIndex].Value);
}
