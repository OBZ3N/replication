#pragma once

#include "zen/debug/zen_debug_assert.h"

namespace zen
{
    namespace serializers
    {
        template <size_t N> struct Log2 { static size_t get() { return 1 + Log2<N / 2>::get(); } };
        template <> struct Log2<0> { static size_t get() { ZEN_ASSERT(false, "invalid call to Log2<0>"); } };
        template <> struct Log2<1> { static size_t get() { return 0; } };
    }
}
