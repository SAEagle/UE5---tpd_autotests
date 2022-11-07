// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UTDTurret.generated.h"

class AUTDProjectile;

UCLASS(Abstract)
class UNREALTOPDOWN_API AUTDTurret : public AActor
{
    GENERATED_BODY()

public:
    AUTDTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 AmmoCount{10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Unists = s))
    float FireFrequency{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AUTDProjectile> Projectile;

private:
    UPROPERTY()
    FTimerHandle FireTimerHandle;

    void OnFire();
};
