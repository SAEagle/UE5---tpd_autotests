// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "UI/PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "TPSGame.UI.PauseMenuShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGamePaused, "TPSGame.UI.FPauseMenuShouldBeCollapsedOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);


using namespace UTPDGame::Test;

namespace
{
void PausePressed(UInputComponent* InputComponent)
{
    if (!InputComponent) return;
    const int32 ActionIndex = GetActionBindingIndexByName(InputComponent, "ToggleGamePause", EInputEvent::IE_Pressed);
    if (ActionIndex != INDEX_NONE)
    {
        const auto JumpActionBind = InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::P);
    }
}
template <class T>
T* FindWidgetByClass()
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetTestGameWorld(), Widgets, T::StaticClass(), false);
    return Widgets.Num() != 0 ? Cast<T>(Widgets[0]) : nullptr;
}

}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);

    return true;
}

bool FPauseMenuShouldBeCollapsedOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    return true;
}

#endif
