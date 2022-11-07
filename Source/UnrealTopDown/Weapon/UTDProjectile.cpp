// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/UTDProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AUTDProjectile::AUTDProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(10.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    check(MovementComponent);
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void AUTDProjectile::BeginPlay()
{
    Super::BeginPlay();

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->OnComponentHit.AddDynamic(this, &AUTDProjectile::OnProjectileHit);
    SetLifeSpan(LifeSeconds);
}

void AUTDProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponentHit, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (GetWorld() && OtherActor)
    {
        MovementComponent->StopMovementImmediately();
        OtherActor->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, this);
        Destroy();
    }
}
