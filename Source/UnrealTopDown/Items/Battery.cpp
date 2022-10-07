// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Battery.h"

using namespace UTPDGame;

constexpr float ChargeAmount = 0.1f;

UTPDGame::Battery::Battery(float PercentIn)
{
    SetPercent(PercentIn);
}

void UTPDGame::Battery::Charge()
{
    SetPercent(Percent + ChargeAmount);
}

void UTPDGame::Battery::UnCharge()
{
    SetPercent(Percent - ChargeAmount);
}

float UTPDGame::Battery::GetPercent() const
{
    return Percent;
}

FColor UTPDGame::Battery::GetColor() const
{
    if (Percent > 0.8f) return FColor::Green;
    if (Percent > 0.3f) return FColor::Yellow;
    return FColor::Red;
}

FString UTPDGame::Battery::ToString() const
{
    return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}

void UTPDGame::Battery::SetPercent(float PercentIn)
{
    Percent = FMath::Clamp(PercentIn, 0.0f, 1.0f);
}
