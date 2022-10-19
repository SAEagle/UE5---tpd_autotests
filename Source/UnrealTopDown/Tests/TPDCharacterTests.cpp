// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/TPDCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "Engine/World.h"
#include "UnrealTopDown/UnrealTopDownCharacter.h"
#include "UnrealTopDown/TPDTypes.h"
#include "Tests/AutomationCommon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LOGCharacterTest, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMighBeChangedWithDamage, "TPSGame.Character.FHealthMighBeChangedWithDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpeWait, "TPSGame.LatentCommand.SimpleWait",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpeLog, "TPSGame.LatentCommand.SimpleLog",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "TPSGame.LatentCommand.OpenCloseMap",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "TPSGame.LatentCommand.FCharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "TPSGame.LatentCommand.FAutoHealShouldRestoreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;

namespace
{
constexpr char* CharacterTestBPName = "Blueprint'/Game/Tests/BP_Test_TopDownCharacter_Child.BP_Test_TopDownCharacter_Child'";

}

bool FHealthMighBeChangedWithDamage::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    AUnrealTopDownCharacter* Character = CreateBlueprintDeferred<AUnrealTopDownCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 100.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    const float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

bool FLatentCommandSimpeWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPDLatentLogCommand, FString, LogMessage);
bool FTPDLatentLogCommand::Update()
{
    UE_LOG(LOGCharacterTest, Display, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpeLog::RunTest(const FString& Parameters)
{
    UE_LOG(LOGCharacterTest, Display, TEXT("Log 1"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPDLatentLogCommand("Latent log 1"));
    UE_LOG(LOGCharacterTest, Display, TEXT("Log 2"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPDLatentLogCommand("Latent log 2"));
    UE_LOG(LOGCharacterTest, Display, TEXT("Log 3"));
    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");  // named object for latent commands
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, AUnrealTopDownCharacter*, Character, float, Lifespan);
bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Lifespan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LOGCharacterTest, Error, TEXT("Character was not destroyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{770.0f, -400.0f, 92.0f}};
    AUnrealTopDownCharacter* Character = CreateBlueprintDeferred<AUnrealTopDownCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 100.0f;
    HealthData.LifeSpanRate = 2.5;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
    Character->FinishSpawning(InitialTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);

    const auto KillingDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KillingDamageAmount, FDamageEvent{}, nullptr, nullptr);

    TestEqual("Health is empty", Character->GetHealthPercent(), 0.0f);
    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);
    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END

    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpanRate));

    ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(Character, HealthData.LifeSpanRate));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, AUnrealTopDownCharacter*, Character, float, HealingDuration);
bool FAutoHealCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LOGCharacterTest, Error, TEXT("Health is not full"));
        }
        return true;
    }
    return false;
}


bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{770.0f, -400.0f, 92.0f}};
    AUnrealTopDownCharacter* Character = CreateBlueprintDeferred<AUnrealTopDownCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;
    FHealthData HealthData;
    HealthData.MaxHealth = 100.0f;
    HealthData.HealModifier = 5.0f;
    HealthData.HealRate = 0.5f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitialTransform);

    const float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier;
    ADD_LATENT_AUTOMATION_COMMAND(FAutoHealCheckLatentCommand(Character, HealingDuration));


    return true;
}

#endif
