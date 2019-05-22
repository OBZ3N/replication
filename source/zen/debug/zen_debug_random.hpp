#pragma once

#include "zen/debug/zen_debug_random.h"

namespace zen
{
    namespace debug
    {
        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE range)
        {
            std::uniform_int_distribution<TYPE> ranged_uniform_distribution(0, range-1);
            TYPE random_value = ranged_uniform_distribution(m_mersenne_generator);
            return random_value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_integer_ranged(TYPE min, TYPE max)
        {
            std::uniform_int_distribution<TYPE> ranged_uniform_distribution(min, max);
            TYPE random_value = ranged_uniform_distribution(m_mersenne_generator);
            return random_value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE range)
        {
            std::uniform_real_distribution<TYPE> uniform_distribution(0, range-1);
            TYPE random_value = uniform_distribution(m_mersenne_generator);
            return random_value;
        }

        template<typename TYPE>
        TYPE Randomizer::get_float_ranged(TYPE min, TYPE max)
        {
            std::uniform_real_distribution<TYPE> uniform_distribution(min, max);
            TYPE random_value = uniform_distribution(m_mersenne_generator);
            return random_value;
        }
    }
}