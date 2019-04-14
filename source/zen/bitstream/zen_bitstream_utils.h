
#pragma once

#include <cinttypes>
#include "zen/debug/zen_debug.h"

//#define STREAMING_ALIGN_TO_BYTES

namespace zen
{
    namespace bitstream
    {
        template<typename TYPE> TYPE align_to_bytes(TYPE num_bits)
        {
            TYPE num_bytes = (num_bits / 8) + ((num_bits & 7) ? 1 : 0);
            return num_bytes * 8;
        }

        // merge a uint32 bitfield into a uint32 output at bit 'position'.
        // returns the number of bits written.
        inline uint32_t merge_bits(uint32_t output, size_t output_position, uint32_t input, size_t input_size, size_t input_offset)
        {
            ZEN_ASSERT(input_offset < 32, "input_offset(", input_size, ") > 32 bits.");
            ZEN_ASSERT(input_size <= 32, "input_size(", input_size, ") > 32 bits.");
            ZEN_ASSERT(output_position < 32, "output_position(", output_position, ") > 32 bits.");

            input = input >> input_offset;
            uint32_t mask = (uint32_t(uint64_t(1) << uint64_t(input_size)) - 1) << output_position;
            uint32_t merged = ((input << output_position) & mask) | (output & ~mask);
            return merged;
        }

        template<typename TYPE>
        inline size_t get_num_words( size_t num_bits )
        {
            size_t word_size = sizeof(TYPE) << 3;
            return num_bits / word_size + (num_bits & (word_size - 1) ) ? 1 : 0;
        }


        inline bool get_bit( uint32_t* bits, size_t i )
        {
            uint32_t value = bits[(i >> 5)] & (1 << (i & 31 ) );

            return (value != 0);
        }

        inline void set_bit( uint32_t* bits, size_t i, bool value )
        {
            if ( value )
            {
                bits[(i >> 5)] |= (1 << (i & 31));
            }
            else
            {
                bits[(i >> 5)] &= ~(1 << (i & 31));
            }
        }
    }
}
