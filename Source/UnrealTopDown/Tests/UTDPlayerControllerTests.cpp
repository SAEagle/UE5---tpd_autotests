// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UTDPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GameFramework/PlayerController.h"
#include "UnrealTopDown/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerController.GameCanBePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerController.GameCanBeUnPaused",
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
}  // namespace

bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PC->IsPaused());
    TestTrueExpr(PC->bShowMouseCursor);

    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(PC->bShowMouseCursor);

    return true;
}

#endif
