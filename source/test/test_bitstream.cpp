
#include "zen/debug/zen_debug_assert.h"
#include "test/test_bitstream.h"

namespace test
{
    Bitstream::Bitstream(zen::debug::Randomizer& randomizer)
        : Base("Bitstream", randomizer)
    {
    }

    void Bitstream::start_internal()
    {
    }

    bool Bitstream::update_internal()
    {
        size_t test_to_run = m_randomizer.get_ranged(4);
        switch (test_to_run)
        {
        default:    return false;
        case 0:     return write();
        case 1:     return poke();
        case 2:     return read();
        case 3:     return peek();
        }
    }

    bool Bitstream::write()
    {
        bool retry;
        do
        {
            // set up the data buffer.
            size_t written_bitcount;
            m_randomizer.get_bits(m_data, written_bitcount, (sizeof(m_data) << 3));
            zen::bitstream::Writer writer(m_data, written_bitcount);
            size_t written_position = writer.bitcount();

            // set up a buffer to append.
            size_t write_size;
            m_randomizer.get_bits(m_temp, write_size, (sizeof(m_temp) << 3));

            ZEN_LOG("    -  write(", write_size, " bits @ ", written_position, ")");

            if (writer.write(m_temp, write_size))
            {
                retry = false;
        
                // validate.
                for (uint32_t i = 0; i < write_size; ++i)
                {
                    ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + written_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + written_position, "] != temp[", i, "].");
                }
            }
            else
            {
                retry = true;
            }
        } 
        while (retry);

        return true;
    }

    bool Bitstream::poke()
    {
        bool retry;
        do
        {
            // set up the data buffer.
            size_t written_bitcount;
            m_randomizer.get_bits(m_data, written_bitcount, (sizeof(m_data) << 3));
            zen::bitstream::Writer writer(m_data, written_bitcount);

            // set up a buffer to poke.
            size_t poke_size;
            m_randomizer.get_bits(m_temp, poke_size, (sizeof(m_temp) << 3));
            size_t poke_position = m_randomizer.get_ranged(writer.bitcount());

            ZEN_LOG("    - poke(", poke_size, " bits @ ", poke_position, ")");

            if (writer.poke(m_temp, poke_size, poke_position))
            {
                retry = false;

                // validate.
                for (uint32_t i = 0; i < poke_size; ++i)
                {
                    ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + poke_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + poke_position, "] != temp[", i, "].");
                }
            }
            else
            {
                retry = true;
            }
        }
        while(retry);

        return true;
    }

    bool Bitstream::read()
    {
        bool retry;
        do
        {
            // sets up a reference buffer.
            size_t written_bitcount;
            m_randomizer.get_bits(m_data, written_bitcount, (sizeof(m_data) << 3));
            size_t read_position = m_randomizer.get_ranged(written_bitcount);
            zen::bitstream::Reader reader(m_data, written_bitcount, read_position);

            // try to read a chunk from the data buffer.
            size_t num_bits_to_read = m_randomizer.get_ranged(reader.bitcount());

            ZEN_LOG("    - read(", num_bits_to_read, " bits @ ", read_position, ")");

            if (reader.read(m_temp, num_bits_to_read))
            {
                retry = false;

                for (uint32_t i = 0; i < num_bits_to_read; ++i)
                {
                    ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + read_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + read_position, "] != temp[", i, "].");
                }
            }
            else
            {
                retry = true;
            }
        }
        while (retry);

        return true;
    }

    bool Bitstream::peek()
    {
        bool retry;
        do
        {
            // sets up a reference buffer.
            size_t written_bitcount;
            m_randomizer.get_bits(m_data, written_bitcount, (sizeof(m_data) << 3));
            size_t read_position = m_randomizer.get_ranged(written_bitcount);
            zen::bitstream::Reader reader(m_data, written_bitcount, read_position);

            // try to read a chunk from the data buffer.
            size_t num_bits_to_read = m_randomizer.get_ranged(reader.bitcount());

            ZEN_LOG("    - peek(", num_bits_to_read, " bits @ ", read_position, ")");

            if (reader.peek(m_temp, num_bits_to_read, read_position))
            {
                retry = false;

                for (uint32_t i = 0; i < num_bits_to_read; ++i)
                {
                    ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + read_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + read_position, "] != temp[", i, "].");
                }
            }
            else
            {
                retry = true;
            }
        }
        while (retry);

        return true;
    }
}
