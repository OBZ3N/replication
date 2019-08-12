#pragma once

#include <limits>
#include "zen/debug/zen_debug_random.h"
#include "zen/debug/zen_debug_assert.h"

namespace zen
{
    namespace debug
    {
        inline double Randomizer::get_random_value_internal()
        {
            uint64_t value = m_uniform_distribution(m_mersenne_generator);

            #undef max
            return (double)value / (double)std::numeric_limits<uint64_t>::max();
        }

        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE range)
        {
            if (range == 0) 
                return 0;

            double rnd = get_random_value_internal();

            TYPE value = (TYPE)(rnd * (range - 1));

            return value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE min_value, TYPE max_value)
        {
            ZEN_ASSERT(max_value >= min_value, "max(", max_value, ") is < min(", min_value, ").");

            double rnd = get_random_value_internal();

            TYPE value = min_value + get_float_ranged(max_value - min_value);

            return value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE range)
        {
            double rnd = get_random_value_internal();

            TYPE value = (TYPE)(rnd * range);

            return value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE min_value, TYPE max_value)
        {
            ZEN_ASSERT(max_value >= min_value, "max(", max_value, ") is < min(", min_value, ").");

            TYPE value = min_value + get_float_ranged(max_value - min_value);

            return value;
        }
    }
}