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

            uint32_t get_ranged(uint32_t range);
            
            void get_bits(uint32_t* bits, size_t& num_bits, size_t max_bits);

        private:
            std::mt19937 m_mersenne_generator;
            std::uniform_int_distribution<uint32_t> m_uniform_distribution;

        };
    }
}
