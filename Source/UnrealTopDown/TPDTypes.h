#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPDTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "MY COOL SPHERE"),
    CUBE UMETA(DisplayName = "MY COOL CUBE"),
    CYLINDER,
    CONE
};

UCLASS()
class UNREALTOPDOWN_API UTDLocalizationFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    static FText GetInventoryItemText(EInventoryItemType ItemType)
    {
        switch (ItemType)
        {
            case EInventoryItemType::SPHERE: return NSLOCTEXT("Inventory", "Sphere_Loc", "SPHERE");
            case EInventoryItemType::CUBE: return NSLOCTEXT("Inventory", "Cube_Loc", "CUBE");
            case EInventoryItemType::CYLINDER: return NSLOCTEXT("Inventory", "Cylinder_Loc", "CYLINDER");
            case EInventoryItemType::CONE: return NSLOCTEXT("Inventory", "Cone_Loc", "CONE");
        }
        checkNoEntry();
        return FText{};
    }
};

USTRUCT(BlueprintType)
struct FInventoryData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInventoryItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;

    FString ToString() const { return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
    float MaxHealth{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float HealModifier{10.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float HealRate{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "s"))
    float LifeSpanRate{5.0f};

    FString ToString() const
    {
        return FString::Printf(TEXT("(MaxHealth=%i,HealModifier=%i, HealRate=%i, LifeSpanRate=%i)"), MaxHealth, HealModifier, LifeSpanRate);
    }
};
