// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealTopDownGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

UCLASS(minimalapi)
class AUnrealTopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealTopDownGameMode();

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
    virtual bool ClearPause();

	UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;
};



