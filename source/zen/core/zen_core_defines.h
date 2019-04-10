#pragma once

#include <cinttypes>

namespace zen
{
    namespace core
    {
        enum class Result
        {
            Ok = 0,
            OutOfMemory,
            PositionOverflow,
            WriteOverflow,
            ReadOverflow,
        };
    }
}