#pragma once

#include "CoreMinimal.h"

namespace UTPDGame
{
namespace Test
{

template <typename Type1, typename Type2>
struct TestPayload
{
    Type1 TestValue;
    Type2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};
}  // namespace Test
}  // namespace UTPDGame