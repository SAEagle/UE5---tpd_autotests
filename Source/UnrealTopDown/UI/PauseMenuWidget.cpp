// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "UnrealTopDownPlayerController.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CloseMButton);
    CloseMButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseMenu);
}

void UPauseMenuWidget::OnCloseMenu()
{
    if (!GetWorld()) return;

    if (auto* PC = Cast<AUnrealTopDownPlayerController>(GetWorld()->GetFirstPlayerController()))
        {
            PC->ToggleGamePause();
        }
}
