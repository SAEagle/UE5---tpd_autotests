#pragma once
#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"

namespace UTPDGame
{
namespace Test
{

UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != NULL))
        {
            return Context.World();
        }
    }

    return nullptr;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString> Params)
{
    if (!Object) return;

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }
    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
{
    if (!InputComp) return INDEX_NONE;

    for (int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
    {
        const FInputActionBinding Action = InputComp->GetActionBinding(i);
        if (Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
        {
            return i;
        }
    }
    return INDEX_NONE;
}

int32 GetAxisBindingByName(UInputComponent* InputComp, const FString& AxisName)
{
    if (!InputComp) return INDEX_NONE;

    // with predicates
    const int32 AxisIndex = InputComp->AxisBindings.IndexOfByPredicate(
        [=](const FInputAxisBinding& AxisBind) { return AxisBind.AxisName.ToString().Equals(AxisName); });

    /* for (int32 i = 0; i < InputComp->AxisBindings.Num(); ++i)
      {
          if (InputComp->AxisBindings[i].AxisName.ToString().Equals(AxisName))
          {
              return i;
          }
      }
        */
    return AxisIndex;
}

FString GetTestDataDir()
{
    return FPaths::GameSourceDir().Append("UnrealTopDown/Tests/Data/");
}

FTPDUntilLatentCommand::FTPDUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool FTPDUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }

    Callback();
    return false;
}

UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
{
    if (!Widget || !Widget->WidgetTree) return nullptr;

    UWidget* FoundWidget = nullptr;
    UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
        [&](UWidget* Child)
        {
            if (Child && Child->GetFName().IsEqual(WidgetName))
            {
                FoundWidget = Child;
            }
        });

    return FoundWidget;
}

void DoInputAction(UInputComponent* InputComponent, const FString& ActionName, const FKey& Key)
{
    if (!InputComponent) return;
    const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, ActionName, EInputEvent::IE_Pressed);
    if (ActionIndex != INDEX_NONE)
    {
        const auto ActionBind = InputComponent->GetActionBinding(ActionIndex);
        ActionBind.ActionDelegate.Execute(Key);
    }
}

void JumpPressed(UInputComponent* InputComponent)
{
    DoInputAction(InputComponent, "Jump", EKeys::SpaceBar);
}

void PausePressed(UInputComponent* InputComponent)
{
    DoInputAction(InputComponent, "ToggleGamePause", EKeys::P);
}

}  // namespace Test
}  // namespace UTPDGame

#endif