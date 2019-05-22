#pragma once

#include <random>
#include <cinttypes>
#include <utility>

namespace zen
{
    namespace debug
    {
        class Randomizer
        {
        public:
            Randomizer();

            void seed(uint32_t seed_value);

            template<typename TYPE> TYPE get_integer_ranged(TYPE range);
            template<typename TYPE> TYPE get_float_ranged(TYPE range);
            template<typename TYPE> TYPE get_integer_ranged(TYPE min, TYPE max);
            template<typename TYPE> TYPE get_float_ranged(TYPE min, TYPE max);

            void get_bits(uint32_t* bits, size_t num_bits);
            void get_bits(uint32_t* bits, size_t& num_bits, size_t max_bits);
            void get_string(std::string& string);

        private:
            std::mt19937 m_mersenne_generator;
        };
    }
}

#include "zen/debug/zen_debug_random.hpp"


