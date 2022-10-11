// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Components/TPDInventoryComponent.h"
#include "UnrealTopDown/TPDTypes.h"
#include "UnrealTopDown/Tests/TestUtils.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "TPSGame.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault, "TPSGame.Components.Inventory.ItemScoresShouldBeZerosByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeItemScoresShouldntBeAdded, "TPSGame.Components.Inventory.NegativeItemScoresShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "TPSGame.Components.Inventory.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCanBeAdded, "TPSGame.Components.Inventory.ScoreMoreThanLimitCanBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;

namespace
{
class UTPDInventoryComponentTestable : public UTPDInventoryComponent
{
public:
    void SetLimits(const TMap<EInventoryItemType, int32>& Limits) { InventoryLimits = Limits; }
};

TMap<EInventoryItemType, int32> InitLimits(UTPDInventoryComponentTestable* InvComp, int32 LimitValue)
{
    TMap<EInventoryItemType, int32> InvLimits;
    ENUM_LOOP_START(EInventoryItemType, EELem)
    InvLimits.Add(EELem, LimitValue);
    ENUM_LOOP_END
    InvComp->SetLimits(InvLimits);
    return InvLimits;
}
}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UTPDInventoryComponent* InvComp = NewObject<UTPDInventoryComponent>();
    if (!TestNotNull("Inventory component exists", InvComp)) return false;

    return true;
}

bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
    const UTPDInventoryComponent* InvComp = NewObject<UTPDInventoryComponent>();
    if (!TestNotNull("Inventory component exists", InvComp)) return false;

    ENUM_LOOP_START(EInventoryItemType, EELem)
    TestTrueExpr(InvComp->GetInventoryAmountByType(EELem) == 0);
    ENUM_LOOP_END

    return true;
}

bool FNegativeItemScoresShouldntBeAdded::RunTest(const FString& Parameters)
{
    UTPDInventoryComponentTestable* InvComp = NewObject<UTPDInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp)) return false;

    InitLimits(InvComp, 100);

    const int32 NegativeScoreAmount = -5;
    const int32 InitialScoreAmount = 10;
    ENUM_LOOP_START(EInventoryItemType, EELem)
    TestTrueExpr(InvComp->TryToAddItem({EELem, InitialScoreAmount}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EELem) == InitialScoreAmount);
    TestTrueExpr(!InvComp->TryToAddItem({EELem, NegativeScoreAmount}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EELem) == InitialScoreAmount);
    ENUM_LOOP_END

    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPDInventoryComponentTestable* InvComp = NewObject<UTPDInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp)) return false;

    const auto InvLimits = InitLimits(InvComp, 100);
   
    ENUM_LOOP_START(EInventoryItemType, EELem)
    for (int32 i = 0; i < InvLimits[EELem]; ++i)
    {
        TestTrueExpr(InvComp->TryToAddItem({EELem, 1}));
        TestTrueExpr(InvComp->GetInventoryAmountByType(EELem) == i + 1);
    }
    ENUM_LOOP_END

    return true;
}

bool FScoreMoreThanLimitCanBeAdded::RunTest(const FString& Parameters)
{
    UTPDInventoryComponentTestable* InvComp = NewObject<UTPDInventoryComponentTestable>();
    if (!TestNotNull("Inventory component exists", InvComp)) return false;

    const int32 ScoreLimit = 100;
    InitLimits(InvComp, ScoreLimit);

    TestTrueExpr(InvComp->TryToAddItem({EInventoryItemType::SPHERE, 10}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);

    TestTrueExpr(!InvComp->TryToAddItem({EInventoryItemType::SPHERE, ScoreLimit + 1}));
    TestTrueExpr(InvComp->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);

    return true;
}

#endif
