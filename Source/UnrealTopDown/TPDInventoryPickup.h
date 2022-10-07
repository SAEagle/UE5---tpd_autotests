// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealTopDown/TPDTypes.h"
#include "TPDInventoryPickup.generated.h"

class USphereComponent;

UCLASS()
class UNREALTOPDOWN_API ATPDInventoryPickup : public AActor
{
    GENERATED_BODY()

public:
    ATPDInventoryPickup();
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryData InventoryData;
};
