
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

        void Randomizer::get_string(std::string& string, size_t max_length)
        {
            string.clear();

            std::uniform_int_distribution<int32_t> uniform_distribution('0', 'z');

            uint32_t num_chars = get_integer_ranged(max_length-1);

            for (size_t i = 0; i < num_chars; ++i)
            {
                int8_t character = (int8_t)uniform_distribution(m_mersenne_generator);
                string.push_back(character);
            }
            string.push_back('\0');
        }

        void Randomizer::get_string(std::string& string, size_t& length, size_t max_length)
        {
            length = get_integer_ranged((size_t)1, max_length);

            get_string(string, length);
        }

        void Randomizer::get_bits(uint32_t* bits, size_t num_bits)
        {
            std::uniform_int_distribution<uint32_t> uniform_distribution;

            uint32_t num_words = (num_bits >> 5);

            for (size_t i = 0; i < num_words; ++i)
            {
                bits[i] = uniform_distribution(m_mersenne_generator);
            }

            if (num_bits & 31)
            {
                uint32_t tail_bits = (num_bits & 31);
                uint32_t mask = ((1 << tail_bits) - 1);
                uint32_t r = uniform_distribution(m_mersenne_generator);
                bits[num_words] = (r & mask) | (bits[num_words] & !mask);
            }
        }
 
        void Randomizer::get_bits(uint32_t* bitfield, size_t& num_bits, size_t max_bits)
        {
            num_bits = get_integer_ranged(max_bits);

            get_bits(bitfield, num_bits);
        }
    }
}