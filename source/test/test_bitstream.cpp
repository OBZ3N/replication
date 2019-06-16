
#include "zen/debug/zen_debug_assert.h"
#include "test/test_bitstream.h"

namespace test
{
    Bitstream::Bitstream(zen::debug::Randomizer& randomizer)
        : Base("Bitstream", randomizer)
        , m_writer(m_data, sizeof(m_data) << 3)
    {
    }

    void Bitstream::start_internal()
    {
        m_writer = zen::bitstream::Writer(m_data, sizeof(m_data) << 3);
    }

    void Bitstream::calculate_valid_tests(std::vector<Bitstream::Test>& tests)
    {
        tests.clear();
        tests.push_back(Test::Write);
        
        if (!m_writer.empty())
        {
            tests.push_back(Test::Poke);
            tests.push_back(Test::Read);
            tests.push_back(Test::Peek);
        }
    }

    bool Bitstream::update_internal()
    {
        std::vector<Test> tests;
        calculate_valid_tests(tests);

        size_t test_to_run = m_randomizer.get_integer_ranged(tests.size());
        switch (tests[test_to_run])
        {
        default:             return false;
        case Test::Write:    return write();
        case Test::Poke:     return poke();
        case Test::Read:     return read();
        case Test::Peek:     return peek();
        }
    }

    bool Bitstream::write()
    {
        size_t write_position = m_writer.bitcount();
        size_t write_size;
        m_randomizer.get_bits(m_temp, write_size, (sizeof(m_temp) << 3));
        
        if (m_writer.write(m_temp, write_size))
        {
            ZEN_LOG("    - write(", write_size, " bits @ ", write_position, ")");

            // validate.
            for (uint32_t i = 0; i < write_size; ++i)
            {
                ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + write_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + write_position, "] != temp[", i, "].");
            }
            m_reader = zen::bitstream::Reader(m_data, m_writer.bitcount(), m_reader.position());
        }
        return true;
    }

    bool Bitstream::poke()
    {
        #undef min
        size_t poke_position = m_randomizer.get_integer_ranged(m_writer.bitcount());
        size_t poke_size;
        size_t poke_capacity = std::min(size_t(m_writer.bitcount() - poke_position), size_t(sizeof(m_temp) << 3));
        m_randomizer.get_bits(m_temp, poke_size, poke_capacity);
        
        if (m_writer.poke(m_temp, poke_size, poke_position))
        {
            ZEN_LOG("    - poke(", poke_size, " bits @ ", poke_position, ")");

            // validate.
            for (uint32_t i = 0; i < poke_size; ++i)
            {
                ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + poke_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + poke_position, "] != temp[", i, "].");
            }
            m_reader = zen::bitstream::Reader(m_data, m_writer.bitcount(), m_reader.position());
        }
        return true;
    }

    bool Bitstream::read()
    {
        // try to read a chunk from the data buffer.
        size_t read_position = m_reader.position();
        size_t num_bits_to_read = m_randomizer.get_integer_ranged(m_reader.get_bits_left(read_position));
        num_bits_to_read = std::min(num_bits_to_read, size_t(sizeof(m_temp) << 3));

        if (m_reader.read(m_temp, num_bits_to_read))
        {
            ZEN_LOG("    - read(", num_bits_to_read, " bits @ ", read_position, ")");

            for (uint32_t i = 0; i < num_bits_to_read; ++i)
            {
                ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + read_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + read_position, "] != temp[", i, "].");
            }
        }
        return true;
    }

    bool Bitstream::peek()
    {
        size_t read_position = m_randomizer.get_integer_ranged(m_reader.bitcount());
        size_t num_bits_to_read = m_randomizer.get_integer_ranged(m_reader.get_bits_left());
        num_bits_to_read = std::min(num_bits_to_read, size_t(sizeof(m_temp) << 3));

        if (m_reader.peek(m_temp, num_bits_to_read, read_position))
        {
            ZEN_LOG("    - peek(", num_bits_to_read, " bits @ ", read_position, ")");

            for (uint32_t i = 0; i < num_bits_to_read; ++i)
            {
                ZEN_ASSERT_RETURN(zen::bitstream::get_bit(m_data, i + read_position) == zen::bitstream::get_bit(m_temp, i), false, "data[", i + read_position, "] != temp[", i, "].");
            }
        }
        return true;
    }
}
