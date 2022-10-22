// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/Components/InputRecordingComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerInput.h"
#include "UnrealTopDown/Tests/JsonUtils.h"

using namespace UTPDGame::Test;

UInputRecordingComponent::UInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    check(GetOwner());

    const APawn* Pawn = Cast<APawn>(GetOwner());
    check(Pawn);

    const auto* PlayerController = Pawn->GetController<APlayerController>();
    check(PlayerController);

    PlayerInput = PlayerController->PlayerInput;
    check(PlayerInput);

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Bindings.Add(MakeBindingsData());
}

void UInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JsonUtils::WriteInputData(GenerateFileName(), InputData);
}

void UInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Bindings.Add(MakeBindingsData());
}

FBindingsData UInputRecordingComponent::MakeBindingsData() const
{
    FBindingsData BindingsData;
    BindingsData.WorldTime = GetWorld()->TimeSeconds;
    for (const auto AxisBinding : GetOwner()->InputComponent->AxisBindings)
    {
        BindingsData.AxisValues.Add(FAxisData{AxisBinding.AxisName, AxisBinding.AxisValue});
    }

    for (int32 i = 0; i < GetOwner()->InputComponent->GetNumActionBindings(); ++i)
    {
        const auto Action = GetOwner()->InputComponent->GetActionBinding(i);
        const auto ActionKeys = PlayerInput->GetKeysForAction(Action.GetActionName());
        if (ActionKeys.Num() > 0)
        {
            // write data
            const bool Pressed = PlayerInput->IsPressed(ActionKeys[0].Key);
            const bool State = (Pressed && Action.KeyEvent == EInputEvent::IE_Pressed) ||  //
                               (!Pressed && Action.KeyEvent == EInputEvent::IE_Released);
            BindingsData.ActionValues.Add(FActionData{Action.GetActionName(), ActionKeys[0].Key, State});
        }
    }

    return BindingsData;
}

FString UInputRecordingComponent::GenerateFileName() const
{
    // return FPaths::GameSourceDir().Append("UnrealTopDown/Tests/Data/CharacterTestInput.json");
    FString SavedDir = FPaths::ProjectSavedDir();
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append("/Tests/").Append("CharacterTestInput").Append("_").Append(Date).Append(".json");
}
