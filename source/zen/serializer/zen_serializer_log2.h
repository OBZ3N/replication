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
            while (value > 0)
            {
                num_bits++;
                value >>= 1;
            }
            return num_bits;
        }

        template<typename TYPE>
        inline size_t number_of_bits_required(TYPE value_min, TYPE value_max)
        {
            ZEN_ASSERT(value_max >= value_min, "value_max(", value_max, ") < value_min(", value_min, ").");

            return number_of_bits_required(size_t(value_max - value_min));
        }
    }
}
