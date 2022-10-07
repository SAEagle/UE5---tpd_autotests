// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "TPSGame.Items.Battery",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
    using namespace UTPDGame;

    AddInfo("Battery with default ctor");
    const Battery BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(BatteryDefault.ToString().Equals("100%"));

    AddInfo("Battery with custom ctor");
    const Battery BatteryObject2{0.54f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject2.GetPercent(), 0.54f));
    TestTrueExpr(BatteryObject2.GetColor() == FColor::Yellow);
    TestTrueExpr(BatteryObject2.ToString().Equals("54%"));

    const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentString)
    {
        const Battery BatteryObject{Percent};
        TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
        TestTrueExpr(BatteryObject.GetColor() == Color);
        TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
    };

    BatteryTestFunc(1.0f, FColor::Green, "100%");
    BatteryTestFunc(0.54f, FColor::Yellow, "54%");
    BatteryTestFunc(0.15f, FColor::Red, "15%");
    BatteryTestFunc(200.f, FColor::Green, "100%");
    BatteryTestFunc(-200.f, FColor::Red, "0%");

    AddInfo("Battery charge/uncharge");
    Battery BatteryObject{0.6f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
    BatteryObject.UnCharge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
    BatteryObject.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));

    AddInfo("Battery corner cases");
    for (int32 i = 0; i < 100; ++i)
    {
        BatteryObject.UnCharge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

    AddInfo("Battery corner cases");
    for (int32 i = 0; i < 100; ++i)
    {
        BatteryObject.Charge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));

    AddInfo("Battery comparison");
    const Battery BatteryLow{0.3f};
    const Battery BatteryHigh{0.9f};

    TestTrueExpr(BatteryHigh >= BatteryLow);
    TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

    AddInfo("Battery in memory  comparison");
    TestNotSame("Not the same batteries", BatteryLow, BatteryHigh);

    const Battery BatteryHighDup{0.9f};
    TestTrueExpr(BatteryHigh == BatteryHighDup);
    TestNotSame("Not the same batteries", BatteryHighDup, BatteryHigh);

    const Battery& BatteryHighRef = BatteryHigh;
    TestSame("The same batteries", BatteryHigh, BatteryHighRef);
    return true;
}

#endif