// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UTDGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "UI/UTDGameUserSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExist, "TPSGame.GameUserSettings.SettingsShouldExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;

bool FSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/TopDown/Maps/CustomLevel");
    TestTrueExpr(UUTDGameUserSettings::Get() != nullptr);
    TestTrueExpr(UUTDGameUserSettings::Get()->GetVideoSettings().Num() == 5);

    return true;
}

#endif
