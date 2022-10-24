// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UnrealTopDownCharacter.h"
#include "Engine/World.h"

AUnrealTopDownPlayerController::AUnrealTopDownPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
}

void AUnrealTopDownPlayerController::ToggleGamePause()
{
    SetPause(!IsPaused());
    bShowMouseCursor = IsPaused();
    IsPaused()                                                                   //
        ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false))  //
        : SetInputMode(FInputModeGameOnly());
}

void AUnrealTopDownPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bInputPressed)
    {
        FollowTime += DeltaTime;

        // Look for the touch location
        FVector HitLocation = FVector::ZeroVector;
        FHitResult Hit;
        if (bIsTouch)
        {
            GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
        }
        else
        {
            GetHitResultUnderCursor(ECC_Visibility, true, Hit);
        }
        HitLocation = Hit.Location;

        // Direct the Pawn towards that location
        APawn* const MyPawn = GetPawn();
        if (MyPawn)
        {
            FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
            MyPawn->AddMovementInput(WorldDirection, 1.f, false);
        }
    }
    else
    {
        FollowTime = 0.f;
    }
}

void AUnrealTopDownPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("SetDestination", IE_Pressed, this, &AUnrealTopDownPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction("SetDestination", IE_Released, this, &AUnrealTopDownPlayerController::OnSetDestinationReleased);
    InputComponent->BindAction("ToggleGamePause", IE_Pressed, this, &ThisClass::ToggleGamePause).bExecuteWhenPaused = true;

    // support touch devices
    InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUnrealTopDownPlayerController::OnTouchPressed);
    InputComponent->BindTouch(EInputEvent::IE_Released, this, &AUnrealTopDownPlayerController::OnTouchReleased);
}

void AUnrealTopDownPlayerController::OnSetDestinationPressed()
{
    // We flag that the input is being pressed
    bInputPressed = true;
    // Just in case the character was moving because of a previous short press we stop it
    StopMovement();
}

void AUnrealTopDownPlayerController::OnSetDestinationReleased()
{
    // Player is no longer pressing the input
    bInputPressed = false;

    // If it was a short press
    if (FollowTime <= ShortPressThreshold)
    {
        // We look for the location in the world where the player has pressed the input
        FVector HitLocation = FVector::ZeroVector;
        FHitResult Hit;
        GetHitResultUnderCursor(ECC_Visibility, true, Hit);
        HitLocation = Hit.Location;

        // We move there and spawn some particles
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
    }
}

void AUnrealTopDownPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    bIsTouch = true;
    OnSetDestinationPressed();
}

void AUnrealTopDownPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    bIsTouch = false;
    OnSetDestinationReleased();
}
