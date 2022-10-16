// My game copyright

#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/TPDInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "UnrealTopDown/TPDInventoryPickup.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "Engine/Blueprint.h"
#include "UnrealTopDown/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealTopDown/UnrealTopDownCharacter.h"
#include "UnrealTopDown/Components/TPDInventoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCanBeCreated, "TPSGame.Items.Inventory.FCppActorCanBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.FBlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.FInventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.FInventoryCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEvenryInventoryItemMeshExists, "TPSGame.Items.Inventory.FEvenryInventoryItemMeshExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame::Test;
namespace
{
constexpr char* InventoryItemBPName = "Blueprint'/Game/Inventory/BP_InventoryPickup.BP_InventoryPickup'";
constexpr char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_InventoryPickup_.BP_Test_InventoryPickup_'";

}  // namespace

// if C++ class would be ABSTRACT we can invert this test, and class would not be spawned
bool FCppActorCanBeCreated::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    const ATPDInventoryPickup* InvItem = World->SpawnActor<ATPDInventoryPickup>(ATPDInventoryPickup::StaticClass(), InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    const ATPDInventoryPickup* InvItem = CreateBlueprint<ATPDInventoryPickup>(World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere comp exists", CollisionComp)) return false;

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
    }

    ENUM_LOOP_END

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Render comp exists", TextRenderComp)) return false;

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh comp exists", StaticMeshComp)) return false;

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPDInventoryPickup* InvItem = CreateBlueprint<ATPDInventoryPickup>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    FVector Vector{120.0f, 130.0f, 140.0f};
    CallFuncByNameWithParams(InvItem, "SetTestData",
        {
            FString::FromInt(132),                                                   //
            FString::SanitizeFloat(354.5f),                                          //
            FString("\"My cool string\""),                                           //
            FString("Yes"),                                                          //
            FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Vector.X, Vector.Y, Vector.Z)  //
        });
    const int32 Score = 13;
    const FInventoryData InvData{EInventoryItemType::CYLINDER, Score};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto TextRendercomp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Render component exists", TextRendercomp)) return false;

    TestTrueExpr(TextRendercomp->Text.ToString().Equals(FString::FromInt(Score)));
    TestTrueExpr(TextRendercomp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh comp exists", StaticMeshComp)) return false;

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exist", Material)) return false;

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPDInventoryPickup* InvItem = CreateBlueprint<ATPDInventoryPickup>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const int32 Score = 13;
    const EInventoryItemType Type = EInventoryItemType::CYLINDER;
    const FInventoryData InvData{Type, Score};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, AUnrealTopDownCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1)) return false;

    const auto Character = Cast<AUnrealTopDownCharacter>(Pawns[0]);
    if (!TestNotNull("Character exists", Character)) return false;

    const auto InvComp = Character->FindComponentByClass<UTPDInventoryComponent>();
    if (!TestNotNull("InvComp exists", InvComp)) return false;
    TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == 0);

    // Character should take inventory item
    Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == Score);
    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(World, UTPDInventoryComponent::StaticClass(), InvItems);
    TestTrueExpr(InvItems.Num() == 0);

    return true;
}

bool FEvenryInventoryItemMeshExists::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ENUM_LOOP_START(EInventoryItemType, EElem)

    const FTransform InitialTransform{FVector{100.0f * (index + 1)}};
    ATPDInventoryPickup* InvItem = CreateBlueprint<ATPDInventoryPickup>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const FInventoryData InvData = {EElem, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh comp exists", StaticMeshComp)) return false;

    const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exists"), *UEnum::GetValueAsString(EElem));

    ENUM_LOOP_END

    return true;
}

#endif
