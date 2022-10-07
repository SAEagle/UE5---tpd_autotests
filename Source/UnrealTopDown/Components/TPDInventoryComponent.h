// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnrealTopDown/TPDTypes.h"
#include "TPDInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALTOPDOWN_API UTPDInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPDInventoryComponent();

    bool TryToAddItem(const FInventoryData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EInventoryItemType, int32> InventoryLimits;

private:
    TMap<EInventoryItemType, int32> Inventory;
};
