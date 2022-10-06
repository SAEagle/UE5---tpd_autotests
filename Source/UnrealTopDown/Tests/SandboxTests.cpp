// Fill out your copyright notice in the Description page of Project Settings.
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/SandboxTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace UTPDGame;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");

    typedef TArray<TestPayload<TInterval<int32>, int32>> MaxIntTestPayload;
    // clanf-format off
    const MaxIntTestPayload TestData{
        {{13, 25}, 25},
        {{13, 25}, 25},
        {{25, 25}, 25},
        {{0, 111}, 111},
        {{0, 0}, 0},
        {{-2345, 0}, 0},
    };
    // clanf-format off

    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
    }

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Sqrt testing");

    typedef TArray<TestPayload<float, float>> SqrtTestPayload;
    // clanf-format off
    const SqrtTestPayload TestData{
        {4.0f, 2.0f},
        {3.0f, 1.7f, 0.1f},
        {9.0f, 3.0f},
    };
    // clanf-format off

    for (const auto Data : TestData)
    {
        const bool IsEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);
        TestTrueExpr(IsEqual);
    }
    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("Sin func testing");

    typedef float Degrees;
    typedef TArray<TestPayload<Degrees, float>> SinTestPayload;
    //clang-format off
    const SinTestPayload TestData{
        {Degrees{0.00f}, 0.0f},
        {Degrees{30.0f}, 0.5f},
        {Degrees{45.0f}, 0.707f},
        {Degrees{60.0f}, 0.866f},
        {Degrees{90.0f}, 1.0f},
    };
    //clang-format on
    for (const auto Data : TestData)
    {
        const float Rad = FMath::DegreesToRadians(Data.TestValue);
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Rad), Data.ExpectedValue, 0.001f));
    }

    return true;
}

#endif