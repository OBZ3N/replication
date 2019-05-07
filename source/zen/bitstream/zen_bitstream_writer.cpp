
#include <algorithm>
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_utils.h"

#undef min
#undef max

namespace zen
{
    namespace bitstream
    {
        Writer::Writer()
            : m_bitfields(nullptr)
            , m_bitcount(0)
            , m_bitcapacity(0)
            , m_last_result(Result::Ok)
        {}

        Writer::Writer(uint32_t* bitfields, size_t bitfield_capacity)
            : m_bitfields(bitfields)
            , m_bitcount(0)
            , m_bitcapacity(bitfield_capacity)
            , m_last_result(Result::Ok)
        {
            ZEN_ASSERT((bitfield_capacity & 31) == 0, "bitfield_capacity(", bitfield_capacity, ") should be a multiple of 64 bits.");
        }

        Writer::~Writer()
        {}

        Writer::Result Writer::get_last_result() const
        {
            return m_last_result;
        }

        void Writer::set_last_result(Result result)
        {
            ZEN_LOG("bitstream::Reader(", this, ") last result set from (", (int)m_last_result, ") to (", (int)result, ").")
                m_last_result = result;
        }

        bool Writer::ok() const
        {
            return m_last_result == Result::Ok;
        }

        void Writer::clear_last_result()
        {
            if (m_last_result != Result::Ok)
            {
                set_last_result(Result::Ok);
            }
        }

        bool Writer::write( const void* bits, size_t num_bits )
        {
        #if defined(STREAMING_ALIGN_TO_BYTES)
            num_bits = align_to_bytes(num_bits);
    #endif

            if ( !write_internal( bits, num_bits, m_bitcount ) )
                return false;

            m_bitcount += num_bits;
            return true;
        }

        bool Writer::poke( const void* bits, size_t num_bits, size_t position )
        {
        #if defined(STREAMING_ALIGN_TO_BYTES)
            num_bits = align_to_bytes(num_bits);
        #endif

            if (position + num_bits > m_bitcount)
            {
                set_last_result(Result::WriteOverflow);
                return false;
            }

            if ( !write_internal( bits, num_bits, position ) )
                return false;

            m_bitcount = std::max(position + num_bits, m_bitcount);
            return true;
        }

        bool Writer::set_bitcount(size_t bitcount)
        {
            if (bitcount > m_bitcount)
                return false;

            m_bitcount = bitcount;

            return true;
        }
                
        bool Writer::overflow(size_t num_bits) const
        {
            return (m_bitcount + num_bits > m_bitcapacity);
        }

        bool Writer::empty() const
        {
            return ( m_bitcount == 0 );
        }


        size_t Writer::bitcount() const
        {
            return m_bitcount;
        }

        void Writer::get_bitfields(void*& bitfields, size_t& bitcount)
        {
            bitfields = m_bitfields;
            bitcount = m_bitcount;
        }

        bool Writer::allocate(uint32_t*& bitfields, size_t& bitcapacity)
        {
            return false;
        }

        bool Writer::write_internal(const void* bits, size_t num_bits, size_t position)
        {
            if (position + num_bits > m_bitcapacity)
            {
                if (!allocate(m_bitfields, m_bitcapacity))
                {
                    set_last_result(Result::WriteOverflow);
                    return false;
                }
            }

            // 32-bit aligned data. push one word at a time.
            if ((uint64_t(bits) & 3) == 0)
            {
                size_t word_size = std::min((size_t)32, num_bits);
                const uint32_t* words = (const uint32_t*)(bits);

                while (num_bits > 0)
                {
                    write_uint32_internal(*words, word_size, position);
                    num_bits -= word_size;
                    position += word_size;
                    word_size = std::min((size_t)32, num_bits);
                    words++;
                }
            }
            // byte-aligned data. push one byte at a time.
            else
            {
                size_t byte_size = std::min((size_t)8, num_bits);
                const uint8_t* byte = (const uint8_t*)(bits);

                while (num_bits > 0)
                {
                    write_uint32_internal((uint32_t)*byte, byte_size, position);
                    num_bits -= byte_size;
                    position += byte_size;
                    byte_size = std::min((size_t)8, num_bits);
                    byte++;
                }
            }
            return true;
        }

        // write a single uint32_t bitfield of length 'num_bits'.
        void Writer:: write_uint32_internal(uint32_t bits, size_t num_bits, size_t position)
        {
            uint32_t& bitfield  = m_bitfields[(position >> 5)];
            size_t offset       = (position & 31);
            size_t length       = std::min(size_t(32) - offset, num_bits);
            
            uint64_t mask = ((uint64_t(1) << uint64_t(length)) - uint64_t(1)) << uint64_t(offset);
            bitfield = ((uint64_t(bits) << uint64_t(offset)) & mask) | (bitfield & ~mask);

            // still need to write the remaining fragment into the next bitfield.
            if ( length < num_bits )
            {
                position += length;
                bits    >>= length;
                num_bits -= length;
                write_uint32_internal(bits, num_bits, position);
            }
        }
    }
}