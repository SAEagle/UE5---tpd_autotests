// My game copyright

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Tests/TestUtils.h"
#include "Weapon/UTDTurret.h"
#include "Misc/AutomationTest.h"

// DEFINE_SPEC(FTestClassName, "Category.TestName", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter |
// EAutomationTestFlags::HighPriority)

BEGIN_DEFINE_SPEC(FTurret, "TPSGame.Turret",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
AUTDTurret* Turret;
const FTransform InitialTransform{FVector{1000.0f}};
END_DEFINE_SPEC(FTurret)

using namespace UTPDGame::Test;

namespace
{
constexpr char* MapName = "/Game/Tests/EmptyTestLevel";
constexpr char* TurretBPName = "Blueprint'/Game/Weapon/BP_UTDTurret.BP_UTDTurret'";
constexpr char* TurretBPTestName = "Blueprint'/Game/Tests/BP_TestUTDTurret.BP_TestUTDTurret'";

void SpecCloseLevel(UWorld* World)
{
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        PC->ConsoleCommand(TEXT("Exit"), true);
    }
}

template <class ObjectClass, class PropertyClass>
PropertyClass GetPropertyValueByName(ObjectClass* Obj, const FString& PropName)
{
    if (!Obj) return PropertyClass();
    for (TFieldIterator<FProperty> PropIt(Obj->StaticClass()); PropIt; ++PropIt)
    {
        const FProperty* Property = *PropIt;
        if (Property)
        {
            UE_LOG(LogTemp, Display, TEXT("%s"), *Property->GetName());
        }
        if (Property && Property->GetName().Equals(PropName))
        {
            return *Property->ContainerPtrToValuePtr<PropertyClass>(Obj);
        }
    }
    return PropertyClass();
}

}  // namespace

void FTurret::Define()
{
    Describe("Create Turret",
        [this]()
        {
            BeforeEach(
                [this]
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                });

            It("Cpp instance can't be created",
                [this]()
                {
                    const FString ExpectedWarnMsg =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *AUTDTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

                    Turret = World->SpawnActor<AUTDTurret>(AUTDTurret::StaticClass(), InitialTransform);
                    TestNull("Turret does not exists", Turret);
                });

            It("Blueprint instance can be created",
                [this]()
                {
                    Turret = CreateBlueprint<AUTDTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            AfterEach([this] { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()
        {
            BeforeEach(
                [this]
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                    Turret = CreateBlueprint<AUTDTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.0f}};
            for (const auto& Data : TestData)
            {
                const auto TestName = FString::Printf(TEXT("Ammo: %i and freq: %.0f should be set up correctly"), Data.Key, Data.Value);

                It(TestName,
                    [this, Data]()
                    {
                        const auto [Ammo, Freq] = Data;
                        CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});
                        const int32 AmmoCount = GetPropertyValueByName<AUTDTurret, int32>(Turret, "AmmoCount");
                        TestTrueExpr(AmmoCount == Ammo);

                        const float FireFrequency = GetPropertyValueByName<AUTDTurret, float>(Turret, "FireFrequency");
                        TestTrueExpr(FireFrequency == Freq);
                    });
            }

            AfterEach([this] { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()
        {
            const int32 InitialAmmo = 3;
            const float FireFreq = 1.0f;
            LatentBeforeEach(
                [this, InitialAmmo, FireFreq](const FDoneDelegate& TestDone)
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                    Turret = CreateBlueprint<AUTDTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                    CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(InitialAmmo), FString::SanitizeFloat(FireFreq)});
                    TestDone.Execute();
                });
;
            LatentIt(FString::Printf(TEXT("Should be empty after %i sec"), FMath::RoundToInt(InitialAmmo * FireFreq)),
                EAsyncExecution::ThreadPool,
                [this, InitialAmmo, FireFreq](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<AUTDTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == InitialAmmo);
                        });

                    const float SyncDelta = 0.5f;
                    FPlatformProcess::Sleep(InitialAmmo * FireFreq + SyncDelta);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<AUTDTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == 0);
                        });

                    TestDone.Execute();
                });

            LatentAfterEach(
                [this](const FDoneDelegate& TestDone)
                {
                    SpecCloseLevel(World);
                    TestDone.Execute();
                });
        });
}

#endif