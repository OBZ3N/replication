
#include <algorithm>
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_utils.h"
#include "zen/debug/zen_debug_assert.h"
#undef min

namespace zen
{
    namespace bitstream
    {
        Reader::Reader()
            : m_bitfields(nullptr)
            , m_bitcount(0)
            , m_position(0)
            , m_last_result(core::Result::Ok)
        {}

        Reader::Reader(const uint32_t* bitfields, size_t bitcount)
            : m_bitfields(bitfields)
            , m_bitcount(bitcount)
            , m_position(0)
            , m_last_result(core::Result::Ok)
        {}

        core::Result Reader::get_last_result() const
        {
            return m_last_result;
        }

        void Reader::set_last_result(core::Result result)
        {
            ZEN_LOG("bitstream::Reader(", this, ") last result set from (", (int)m_last_result, ") to (", (int)result, ").")
            m_last_result = result;
        }

        bool Reader::ok() const 
        { 
            return m_last_result == core::Result::Ok;
        }

        void Reader::clear_last_result()
        {
            if (m_last_result != core::Result::Ok)
            {
                set_last_result(core::Result::Ok);
            }
        }

        bool Reader::peek( void* bits, size_t num_bits, size_t position )
        {
            return read_internal( bits, num_bits, position );
        }

        bool Reader::read(void* bits, size_t num_bits)
        {
            if(!read_internal(bits, num_bits, m_position) )
                return false;

            m_position += num_bits;
            return true;
        }
        
        bool Reader::read_internal(void* bits, size_t num_bits, size_t position)
        {
            // reading past the data.
            if (m_position + num_bits > m_bitcount)
            {
                set_last_result(core::Result::ReadOverflow);
                return false;
            }

            // 32-bit aligned data. push one word at a time.
            if ((uint64_t(bits) & 3) == 0)
            {
                size_t word_size = std::min((size_t)32, num_bits);
                uint32_t* word = (uint32_t*)(bits);

                while (num_bits > 0)
                {
                    read_uint32_internal(*word, word_size, position);
                    num_bits -= word_size;
                    position += word_size;
                    word_size = std::min((size_t)32, num_bits);
                    word++;
                }
            }
            // byte-aligned data. push one byte at a time.
            else
            {
                size_t byte_size = std::min((size_t)8, num_bits);
                uint8_t* byte = (uint8_t*)(bits);

                while (num_bits > 0)
                {
                    uint32_t temp;
                    read_uint32_internal(temp, byte_size, position);
                    
                    *byte = uint8_t(temp);
                    num_bits -= byte_size;
                    position += byte_size;
                    byte_size = std::min((size_t)8, num_bits);
                    byte++;
                }
            }
            return true;
        }

        bool Reader::set_position(size_t position)
        {
            if (position > m_bitcount)
            {
                set_last_result(core::Result::PositionOverflow);
                return false;
            }

            m_position = position;
            return true;
        }

        bool Reader::eof() const
        {
            return (m_position >= m_bitcount);
        }

        size_t Reader::bitcount() const
        {
            return m_bitcount;
        }

        bool Reader::overflow(size_t num_bits) const
        {
            return (m_position + num_bits > m_bitcount);
        }

        size_t Reader::position() const
        {
            return m_position;
        }

        void Reader::get_buffer(const void*& bitfields, size_t& bitcount)
        {
            bitfields = m_bitfields;

            bitcount = m_bitcount;
        }

        // push values into cache, one uint32 at a time.
        void Reader::read_uint32_internal(uint32_t& bits, size_t num_bits, size_t position)
        {
            ZEN_ASSERT((num_bits <= 32), "num_bits(", num_bits, ") > 32.");

            uint32_t bitfield   = m_bitfields[(position >> 5)];
            size_t offset       = (position & 31);
            size_t length       = std::min(size_t(32) - offset, num_bits);            
            bits = merge_bits(bits, 0, bitfield, length, offset);

            // still need to read the remaining fragment from the next bitfield.
            if ( length < num_bits )
            {
                uint32_t bitfield  = m_bitfields[((position + length) >> 5)];
                offset = length;
                length = num_bits - length;
                bits = merge_bits(bits, offset, bitfield, length, 0);
            }
        }
    }
}