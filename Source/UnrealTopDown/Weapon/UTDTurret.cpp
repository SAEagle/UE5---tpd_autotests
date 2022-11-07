// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/UTDTurret.h"
#include "Weapon/UTDProjectile.h"
#include "Components/StaticMeshComponent.h"

AUTDTurret::AUTDTurret()
{
    PrimaryActorTick.bCanEverTick = false;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    check(TurretMesh);
    SetRootComponent(TurretMesh);
}

void AUTDTurret::BeginPlay()
{
    Super::BeginPlay();

    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AUTDTurret::OnFire, FireFrequency, true, FirstDelay);
}


void AUTDTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    if (GetWorld())
    {
        const auto SocketTransform = TurretMesh->GetSocketTransform("Muzzle");
        auto* ProjectileObj = GetWorld()->SpawnActorDeferred<AUTDProjectile>(Projectile, SocketTransform);
        if (ProjectileObj)
        {
            ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
            ProjectileObj->FinishSpawning(SocketTransform);
        }
    }
}
