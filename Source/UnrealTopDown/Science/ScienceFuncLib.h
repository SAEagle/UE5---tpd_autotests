// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"

UCLASS()
class UNREALTOPDOWN_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Science)
    static int32 Fibonacci(int32 Value);
};
