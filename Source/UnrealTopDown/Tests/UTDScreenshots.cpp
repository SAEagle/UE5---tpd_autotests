// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UTDScreenshots.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "Camera/CameraActor.h"
#include "BufferVisualizationData.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "TPSGame.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMAinPlayerHUDShouldBeRendered, "TPSGame.Screenshots.MainPlayerHUDShouldBeRendered",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
    "TPSGame.Screenshots.FHealthWidgetShouldBeRenderedCorrectlyAfterDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetRenderedOnPause, "TPSGame.Screenshots.FSettingsWidgetRenderedOnPause",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;

//later move it to TestUtils.h
class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
{
public:
    FTakeScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
    {
        FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(
            this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
    }

    virtual ~FTakeScreenshotLatentCommand()
    {  //
        FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
    }

    virtual bool Update() override
    {
        if (!ScreenshotRequested)
        {
            const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
            UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetTestGameWorld(), ScreenshotName, FString{}, Options);
            ScreenshotRequested = true;
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Frame %i"), GFrameCounter);
        }

        return CommandCompleted;
    }

private:
    const FString ScreenshotName;
    bool ScreenshotRequested{false};
    bool CommandCompleted{false};

    void OnScreenshotTakenAndCompared() { CommandCompleted = true; }
};

// later move it to TestUtils.h
class FTakeUIScreenshotLatentCommand : public IAutomationLatentCommand
{
public:
    FTakeUIScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
    {
        FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(
            this, &FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared);
    }

    virtual ~FTakeUIScreenshotLatentCommand()
    {  //
        FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
    }

    virtual bool Update() override
    {
        if (!ScreenshotSetupDone)
        {
            ScreenshotSetupDone = true;
            SetBufferVisualization("Opacity");
            return false;
        }

        if (!ScreenshotRequested)
        {
            const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
            UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetTestGameWorld(), ScreenshotName, Options);
            ScreenshotRequested = true;
        }
        else
        {
            UE_LOG(LogTemp, Display, TEXT("Frame %i"), GFrameCounter);
        }
        return CommandCompleted;
    }

private:
    const FString ScreenshotName;
    bool ScreenshotRequested{false};
    bool CommandCompleted{false};
    bool ScreenshotSetupDone{false};

    void OnScreenshotTakenAndCompared()
    {
        CommandCompleted = true;
        SetBufferVisualization(NAME_None);
    }

    void SetBufferVisualization(const FName& VisualizeBuffer)
    {
        if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
        {
            static IConsoleVariable* ICVar =
                IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
            if (ICVar)
            {
                if (ViewportClient->GetEngineShowFlags())
                {
                    ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(VisualizeBuffer == NAME_None ? false : true);
                    ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer == NAME_None ? true : false);
                    ICVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
                }
            }
        }
    }
};

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform Transform{FVector{-1470.0f, 1550.0f, 582.0f}};
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera exists", Camera)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exists", PC)) return false;

    PC->SetViewTarget(Camera);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeScreenshotLatentCommand("redering_check_screenshot"));
    return true;
}

bool FMAinPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("mainui_check_screenshot"));
    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exists", PC)) return false;
    if (!TestNotNull("Pawn exists", PC->GetPawn())) return false;

    const float DamageAmount = 35.0f;
    PC->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_check_screenshot"));
    return true;
}

bool FSettingsWidgetRenderedOnPause::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/TopDownMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC exists", PC)) return false;

    PausePressed(PC->InputComponent);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_check_screenshot"));
    return true;
}

#endif
