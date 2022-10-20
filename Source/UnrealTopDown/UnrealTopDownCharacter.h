// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPDTypes.h"
#include "UnrealTopDownCharacter.generated.h"

class UTPDInventoryComponent;
class UDamageType;
class AController;
class USpringArmComponent;
class UCameraComponent;

UCLASS(Blueprintable)
class AUnrealTopDownCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUnrealTopDownCharacter();

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;

    /** Returns TopDownCameraComponent subobject **/
    UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    UFUNCTION(BlueprintCallable, Category = "Health")  // for protected  meta = (BlueprintProtected = "true")
    float GetHealthPercent() const;

private:
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    float Health{0.0f};
    FTimerHandle HealTimerHandle;

    void OnHealing();
    void OnDeath();

    UFUNCTION()
    void OnAnyDamageReceived(
        AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InsitgatedBy, AActor* DamageCauser);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UTPDInventoryComponent* InventoryComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    FHealthData HealthData;

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;
};
