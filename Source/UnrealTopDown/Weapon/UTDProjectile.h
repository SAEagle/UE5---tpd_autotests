// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UTDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)  // can't create C++ object, but only for the BP
class UNREALTOPDOWN_API AUTDProjectile : public AActor
{
    GENERATED_BODY()

public:
    AUTDProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount{30.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Units = s))
    float LifeSeconds{5.0f};

    virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
