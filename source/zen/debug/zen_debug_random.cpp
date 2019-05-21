
#include <limits>
#include "zen/debug/zen_debug_random.h"
#undef max

namespace zen
{
    namespace debug
    {

        Randomizer::Randomizer()
            : m_uniform_distribution(0, std::numeric_limits<uint32_t>::max())
        {}

        void Randomizer::seed( uint32_t seed_value )
        {
            m_mersenne_generator.seed(seed_value);
        }

        void Randomizer::get_bits( uint32_t* bitfield, size_t& num_bits, size_t max_bits )
        {
            num_bits = get_integer_ranged(max_bits);
            
            uint32_t num_words = std::min((num_bits >> 5), (max_bits >> 5));

            for ( size_t i = 0; i < num_words; ++i )
            {
                bitfield[i] = (uint32_t)m_uniform_distribution(m_mersenne_generator);
            }

            uint32_t tail_bits = ( num_bits & 31 );

            bitfield[num_words] = (uint32_t)m_uniform_distribution(m_mersenne_generator);
            bitfield[num_words] &= (( 1 << tail_bits ) - 1);
        }
    }
}