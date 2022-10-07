// Fill out your copyright notice in the Description page of Project Settings.

#include "TPDInventoryPickup.h"
#include "GameFrameWork/Pawn.h"
#include "Components/SphereComponent.h"
#include "UnrealTopDown/Components/TPDInventoryComponent.h"

ATPDInventoryPickup::ATPDInventoryPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(30.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ATPDInventoryPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InvComp = Pawn->FindComponentByClass<UTPDInventoryComponent>())
        {
            if (InvComp->TryToAddItem(InventoryData))
            {
                Destroy();
            }
        }
    }
}
