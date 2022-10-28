// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "UI/PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/VideoSettingsWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "UI/UTDGameUserSettings.h"
#include "UI/SettingOptionWidget.h"
#include "UI/UTDGameSetting.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "TPSGame.UI.PauseMenuShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGamePaused, "TPSGame.UI.FPauseMenuShouldBeCollapsedOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVideoSettingExist, "TPSGame.UI.AllVideoSettingExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsCanBeAplied, "TPSGame.UI.SettingsCanBeAplied",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldWork, "TPSGame.UI.FAutoBenchmarkShouldWork",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

using namespace UTPDGame::Test;

namespace
{

void NextSettingClick(int32 SettingIndex)
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const auto* SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
    const auto* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
    NextSettingButton->OnClicked.Broadcast();
}

void DoBenchmarkClick()
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    BenchmarkButton->OnClicked.Broadcast();
}
}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
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

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    return true;
}

bool FVideoSettingExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BenchmarkButton != nullptr);

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VerticalBox != nullptr);

    const auto& VideoSettings = UUTDGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSettings.Num());
    TestTrueExpr(VerticalBox->GetChildrenCount() > 0);

    return true;
}

bool FSettingsCanBeAplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    const auto& VideoSettings = UUTDGameUserSettings::Get()->GetVideoSettings();

    const int32 GIQualityBefore = UUTDGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore == VideoSettings[2]->GetCurrentOption().Value);

    NextSettingClick(2);
    const int32 GIQualityAfter = UUTDGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore != GIQualityAfter);
    TestTrueExpr(GIQualityAfter == VideoSettings[2]->GetCurrentOption().Value);

    UUTDGameUserSettings::Get()->SetGlobalIlluminationQuality(GIQualityBefore);

    /*
    for (int32 i = 0; i < 5; ++i)
    {
        ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
            [&, i]()
            {
                NextSettingClick(i);
                return true;
            }));
    }
    */
    return true;
}

bool FAutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    DoBenchmarkClick();

    TArray<int32> SavedSettingValues;
    const auto& VideoSettings = UUTDGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings)
    {
        SavedSettingValues.Add(Setting->GetCurrentOption().Value);
    }

    // random clicks
    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        NextSettingClick(i);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] != VideoSettings[i]->GetCurrentOption().Value);
    }

    DoBenchmarkClick();

    
    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] == VideoSettings[i]->GetCurrentOption().Value);
    }


    return true;
}

#endif
