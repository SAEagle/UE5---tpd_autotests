// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/UTDGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealTopDown/TPDInventoryPickup.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Tests/JsonUtils.h"
#include "UnrealTopDown/Tests/InputRecordingUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.GamePlay.FInventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.GamePlay.FInventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsCanBeTakenOnMovement, "TPSGame.GamePlay.FAllItemsCanBeTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsCanBeTakenOnRecordingMovement, "TPSGame.GamePlay.FAllItemsCanBeTakenOnRecordingMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded, "TPSGame.GamePlay.FMapsShouldBeLoaded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);

    return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel_01");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f));

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel_02");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f));

    return true;
}

bool FAllItemsCanBeTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel_03");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 7);

    const int32 MoveForwardIndex = GetAxisBindingByName(Character->InputComponent, "MoveForward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);
    const int32 MoveRightIndex = GetAxisBindingByName(Character->InputComponent, "MoveRight");
    TestTrueExpr(MoveRightIndex != INDEX_NONE);

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPDUntilLatentCommand([=]()                                      //
        { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); },  //
        []() {},                                                                                    //
        3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPDUntilLatentCommand([=]()                                    //
        { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); },  //
        []() {},                                                                                  //
        2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
        : World(InWorld), InputComponent(InInputComponent), BindingsData(InBindingsData)
    {
    }

    virtual bool Update() override
    {
        if (!World || !InputComponent) return true;

        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->TimeSeconds;
        }
        // for will not pass on a different framefrates!!! Please use while
        while (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
            {
                const float AxisValue = BindingsData[Index].AxisValues[i].Value;
                InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
            }

            if (Index > 0)
            {
                for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
                {
                    const auto ActionValue = BindingsData[Index].ActionValues[i];
                    const auto PrevActionValue = BindingsData[Index - 1].ActionValues[i];
                    if (ActionValue.State && ActionValue.State != PrevActionValue.State)
                    {
                        InputComponent->GetActionBinding(i).ActionDelegate.Execute(ActionValue.Key);
                    }
                }
            }
            if (++Index >= BindingsData.Num()) return true;
        }
        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> BindingsData;
    int32 Index{0};
    float WorldStartTime{0.0f};
};

void FAllItemsCanBeTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeatifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"MainMap", "/Game/TopDown/Maps/TopDownMap", "CharacterTestInputMain.json"},       //
            {"CustomMap", "/Game/TopDown/Maps/CustomLevel", "CharacterTestInputCustom.json"},  //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeatifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

bool FAllItemsCanBeTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map name and Json params not valid", ParsedParams.Num() == 2)) return false;

    const auto Level = LevelScope(ParsedParams[0]);

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 4);

    FString FileName =
        GetTestDataDir().Append(ParsedParams[1]);  // FPaths::GameSourceDir().Append("UnrealTopDown/Tests/Data/CharacterTestInput.json");

    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player Controller exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPDInventoryPickup::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeatifiedNames, TArray<FString>& OutTestCommands) const
{
    // const TTuple<FString, int32, bool, float> MyTuple1 = {"unreal", 5, true, 100.0f};
    // const auto MyTuple2 = MakeTuple(FString("unity"), 7, false, 50.0f);

    // auto str = MyTuple1.Get<0>();
    // auto intgr = MyTuple1.Get<1>();
    // auto bln = MyTuple1.Get<2>();
    // auto flt = MyTuple1.Get<3>();

    // UE_LOG(LogTemp, Display, TEXT("TUPLE1: %s, %i, %i, %f"), *str, intgr, bln, flt);

    // Tie(str, intgr, bln, flt) = MyTuple2;
    // UE_LOG(LogTemp, Display, TEXT("TUPLE2: %s, %i, %i, %f"), *str, intgr, bln, flt);

    const TArray<TTuple<FString, FString>> TestData =  //
        {
            {"MainMap", "/Game/TopDown/Maps/TopDownMap"},     //
            {"CustomMap", "/Game/TopDown/Maps/CustomLevel"},  //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeatifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    return true;
}
#endif
