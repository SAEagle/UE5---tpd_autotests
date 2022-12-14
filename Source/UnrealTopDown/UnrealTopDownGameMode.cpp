// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTopDownGameMode.h"
#include "UnrealTopDownPlayerController.h"
#include "UnrealTopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealTopDownGameMode::AUnrealTopDownGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUnrealTopDownPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

bool AUnrealTopDownGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool Succeed = Super::SetPause(PC, CanUnpauseDelegate);
    if (Succeed)
    {
        OnGamePause.Broadcast(true);
	}
    return false;
}

bool AUnrealTopDownGameMode::ClearPause()
{
    const bool Succeed = Super::ClearPause();
    if (Succeed)
    {
        OnGamePause.Broadcast(false);
    }
    return false;
}
