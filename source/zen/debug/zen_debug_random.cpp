
#include <limits>
#include "zen/debug/zen_debug_random.h"
#undef max

namespace zen
{
    namespace debug
    {

        Randomizer::Randomizer()
        {}

        void Randomizer::seed( uint32_t seed_value )
        {
            m_mersenne_generator.seed(seed_value);
        }
        
        void Randomizer::get_bits(uint32_t* bits, size_t num_bits)
        {
            std::uniform_int_distribution<uint32_t> uniform_distribution;

            uint32_t num_words = std::min((num_bits >> 5), (max_bits >> 5));
            for (size_t i = 0; i < num_words; ++i)
            {
                bitfield[i] = uniform_distribution(m_mersenne_generator);
            }

            uint32_t tail_bits = (num_bits & 31);
            bitfield[num_words] = uniform_distribution(m_mersenne_generator);
            bitfield[num_words] &= ((1 << tail_bits) - 1);
        }

        void Randomizer::get_bits( uint32_t* bitfield, size_t& num_bits, size_t max_bits )
        {
            num_bits = get_integer_ranged(max_bits);

            get_bits(bitfield, num_bits);
        }

        void Randomizer::get_string(std::string& string, size_t max_length)
        {
            string.clear();

            std::uniform_int_distribution<int8_t> uniform_distribution('0', 'z');

            uint32_t num_chars = get_integer_ranged(max_length-1);

            for (size_t i = 0; i < num_chars; ++i)
            {
                int8_t character = uniform_distribution(m_mersenne_generator);
                string.push_back(character);
            }
            string.push_back('\0');
        }

        void Randomizer::get_bits(uint32_t* bitfield, size_t num_bits)
        {
            uint32_t num_words = num_bits >> 3;

            for (size_t i = 0; i < num_words; ++i)
            {
                bitfield[i] = (uint32_t)m_uniform_distribution(m_mersenne_generator);
            }

            uint32_t tail_bits = (num_bits & 31);

            bitfield[num_words] = (uint32_t)m_uniform_distribution(m_mersenne_generator);
            bitfield[num_words] &= ((1 << tail_bits) - 1);
        }
    }
}