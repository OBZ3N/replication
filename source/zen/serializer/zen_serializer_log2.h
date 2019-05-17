#pragma once

#include "zen/debug/zen_debug_assert.h"

namespace zen
{
    namespace serializers
    {
        template <size_t N> struct Log2 { static size_t get() { return 1 + Log2<N / 2>::get(); } };
        template <> struct Log2<0> { static size_t get() { ZEN_ASSERT(false, "invalid call to Log2<0>"); } };
        template <> struct Log2<1> { static size_t get() { return 0; } };

        inline size_t number_of_bits_required(size_t value)
        {
            size_t num_bits = 0;
            if (value >= 0x7FFFFFFF) { value >>= 32; num_bits += 32; }
            if (value >= 0x7FFF) { value >>= 16; num_bits += 16; }
            if (value >= 0x7F) { value >>= 8; num_bits += 8; }
            if (value >= 0x7) { value >>= 4; num_bits += 4; }
            if (value >= 0x3) { value >>= 2; num_bits += 2; }
            if (value >= 0x1) { value >>= 1; num_bits += 1; }
            return num_bits;
        }

        inline size_t number_of_bits_required(size_t value_min, size_t value_max)
        {
            ZEN_ASSERT(value_max >= value_min, "value_max(", value_max, ") < value_min(", value_min, ").");

            return number_of_bits_required(value_max - value_min);
        }
    }
}
