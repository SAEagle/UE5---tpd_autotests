#pragma once
#if WITH_AUTOMATION_TESTS

#include "UnrealTopDown/Tests/TestUtils.h"

namespace UTPDGame
{
namespace Test
{

UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != NULL))
        {
            return Context.World();
        }
    }

    return nullptr;
}
}  // namespace Test
}  // namespace UTPDGame

#endif