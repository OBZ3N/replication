#pragma once

#include "zen/debug/zen_debug_random.h"

namespace zen
{
    namespace debug
    {
        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE range)
        {
            if (range == 0)
                return 0;

            uint32_t random_value = (uint32_t)m_uniform_distribution(m_mersenne_generator);

            uint32_t value = random_value % range;

            return value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE min, TYPE max)
        {
            if (max < min)
            {
                return get_integer_ranged(min - max + 1);
            }
            else
            {
                return get_integer_ranged(max - min + 1);
            }
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE range)
        {
            uint32_t random_value = (uint32_t)m_uniform_distribution(m_mersenne_generator);
            uint32_t random_range = std::numeric_limits<uint32_t>::max();
            TYPE ratio = TYPE(random_value) / TYPE(random_range);
            return range * ratio;
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE min, TYPE max)
        {
            if (max < min)
            {
                return get_float_ranged(min - max);
            }
            else
            {
                return get_float_ranged(max - min);
            }
        }
    }
}