// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealTopDown/Tests/SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPDGame.Math.MaxInt",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPDGame.Math.Sqrt",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathRandRange, "TPDGame.Math.RandRange",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	if (1 != 2)
	{
		AddWarning("1 not euqal 2");
		return true;
	}

	TestTrue("2 different positive numbers", FMath::Max(12, 25) == 25);
	TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
	TestTrueExpr(FMath::Max(0, 123) == 123);
	TestTrue("2 zeroes", FMath::Max(0, 0) == 0);
	TestTrue("2 equal negative numbers", FMath::Max(-9, -9) == -9);
	TestTrue("2 different negative numbers", FMath::Max(-45, -84) == -45);

	// 13, 25
	// 25, 25
	// 0, 123
	// 0, 0
	// -235, 0
	// -45, -84
	// -9, -9
	// -5, 5

	return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	// sqrt(3) = 1.732

	AddInfo("Sqrt function testing");

	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
	//TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f);
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f, 0.1f);
	TestEqual("Sqrt(3) [2]", FMath::Sqrt(3.0f), 1.73f, 0.01f);
	return true;
}

bool FMathRandRange::RunTest(const FString& Parameters)
{
	TestEqual("Random Range 1 [0]", FMath::RandRange(0, 2), 1);

	return true;
}


