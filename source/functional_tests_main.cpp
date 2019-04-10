
#include <algorithm>
#include <thread>
#include <chrono>
#include <cinttypes>
#include <string>
#include <vector>
#include <unordered_map>

#include "zen/debug/zen_debug.h"
#include "zen/data/zen_data_vector.h"
#include "zen/data/zen_data_integer.h"
#include "zen/data/zen_data_vector.h"
#include "zen/bitstream/zen_bitstream_utils.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

#undef min

std::list<uint32_t> array_ref[2];
zen::data::Vector<zen::data::UInt32> array_zen[2];
zen::debug::Randomizer randomizer;
uint32_t array_value[2];
uint32_t test_iteration = 0;
uint32_t test_number = 0;
uint32_t num_tests = 0;

uint32_t serialisation_buffer[10000];
uint32_t bitfield_writer_data[10000];
uint32_t bitfield_reader_data[10000];
uint32_t bitfield_writer_checker[10000];
zen::bitstream::Writer bitfield_writer;
zen::bitstream::Reader bitfield_reader;

void verify_delta_compression()
{
    zen::bitstream::Writer writer(serialisation_buffer, sizeof(serialisation_buffer) << 3);

    array_zen[0].serialize_delta( array_zen[1], writer );

    ZEN_ASSERT(writer.ok(), "    ", test_number, "/", num_tests, " stream writer error : ", (uint32_t)writer.get_last_result(), ".");

    if( !writer.empty() )
    {
        zen::data::Vector<zen::data::UInt32> array_copy = array_zen[1];
 
        zen::bitstream::Reader reader(serialisation_buffer, writer.bitcount());

        array_copy.deserialize_delta( array_zen[1], reader );

        array_copy.sanity_check();

        ZEN_ASSERT(reader.ok(), "    ", test_number, "/", num_tests, " stream reader error : ", (uint32_t)reader.get_last_result(), ".");

        ZEN_ASSERT( reader.eof(), "    ", test_number, "/", num_tests, " delta compression reader has data remaining : ", (reader.bitcount() - reader.position()), " bits reaming" );

        ZEN_ASSERT( array_copy == array_zen[0], "    ", test_number, "/", num_tests, " delta compression serialisation mistmatch" );
    }
}

void verify_arrays()
{
    for(size_t t = 0; t < 2; ++t )
    {
        ZEN_ASSERT(array_ref[t].size() == array_zen[t].size(), "    ", test_number, "/", num_tests, "array size mismatch : ref(", array_ref[t].size(), "), zen(", array_zen[t].size(), ")");
    
        for ( size_t i = 0; i < std::min( array_ref[t].size(), array_zen[t].size() ); ++i )
        {
            std::list<uint32_t>::iterator it = array_ref[t].begin();
            std::advance( it, i );

            ZEN_ASSERT((*it) == array_zen[t][i], "    ", test_number, "/", num_tests, "array element[", i, "] mismatch : ref(", (*it), "), zen(", array_zen[t][i], ")");
        }
        
        array_zen[t].sanity_check();
    }

    verify_delta_compression();
}

void verify_bitfield_writer()
{
    for( uint32_t i = 0; i < bitfield_writer.bitcount(); ++i )
    {
        ZEN_ASSERT(zen::bitstream::get_bit(bitfield_writer_data, i) == zen::bitstream::get_bit(bitfield_writer_checker, i), "streamed_bitfield[", i, "] != check_bitfield[", i, "].");
    }
}

void test_bitstream_read()
{
    ZEN_LOG("    ", test_number, "/", num_tests, " test_bitstream_read(", bitfield_reader.position(), ")");

    uint32_t bits[1000];
    size_t num_bits = randomizer.get_ranged(sizeof(bits) << 3);
    size_t position = bitfield_reader.position();
    size_t read_offset = bitfield_reader.position();

    if ( bitfield_reader.read( bits, num_bits ))
    {
        for( uint32_t i = 0; i < num_bits; ++i )
        {
            ZEN_ASSERT(zen::bitstream::get_bit(bitfield_reader_data, i + read_offset) == zen::bitstream::get_bit(bits, i), "bitfield[", i, "] != read_bitfield[", i, "].");
        }
    }
}

void test_bitstream_peek()
{
    uint32_t bits[1000];
    size_t position = randomizer.get_ranged(bitfield_reader.bitcount());
    size_t num_bits = randomizer.get_ranged(sizeof(bits) << 3);
    
    ZEN_LOG("    ", test_number, "/", num_tests, " test_bitstream_peek(", num_bits, " bits @ ", position,")");

    if ( bitfield_reader.peek( bits, num_bits, position ))
    {
        for( uint32_t i = 0; i < num_bits; ++i )
        {
            ZEN_ASSERT(zen::bitstream::get_bit(bitfield_reader_data, i + position) == zen::bitstream::get_bit(bits, i), "bitfield[", i, "] != peeked_bitfield[", i, "].");
        }
    }
}

void test_bitstream_poke()
{
    memcpy(bitfield_writer_checker, bitfield_writer_data, sizeof(bitfield_writer_checker));

    size_t position = randomizer.get_ranged(bitfield_writer.bitcount());

    uint32_t bits[100];
    size_t num_bits;
    randomizer.get_bits( bits, num_bits, (sizeof(bits) << 3) );

    ZEN_LOG("    ", test_number, "/", num_tests, " test_bitstream_poke(", num_bits, " bits @ ", position,")");

    if(bitfield_writer.poke(bits, num_bits, position))
    {
        for( uint32_t i = 0; i < num_bits; ++i )
        {
            zen::bitstream::set_bit(bitfield_writer_checker, position + i, zen::bitstream::get_bit(bits, i));
        }
    }
    verify_bitfield_writer();
}

void test_bitstream_write()
{
    uint32_t bits[100];
    size_t num_bits;
    randomizer.get_bits( bits, num_bits, (sizeof(bits) << 3) );
    
    ZEN_LOG("    ", test_number, "/", num_tests, " test_bitstream_write(", num_bits, ")");

    size_t bitfield_size = bitfield_writer.bitcount();

    if(bitfield_writer.write(bits, num_bits))
    {
        for( uint32_t i = 0; i < num_bits; ++i )
        {
            zen::bitstream::set_bit(bitfield_writer_checker, i + bitfield_size, zen::bitstream::get_bit(bits, i));
        }
    }
    verify_bitfield_writer();
}

void test_bitstream_writer_poke()
{

}

void test_push_front()
{
    for(size_t t = 0; t < 2; ++t )
    {
        array_value[t]++;
        ZEN_LOG("    ", test_number, "/", num_tests, " test_push_front(", array_value[t], ")");
        array_ref[t].push_front(array_value[t]);
        array_zen[t].push_front(zen::data::UInt32(array_value[t]));
        verify_arrays();
    }
}

void test_push_back()
{
    for(size_t t = 0; t < 2; ++t )
    {
        array_value[t]++;
        ZEN_LOG("    ", test_number, "/", num_tests, " test_push_back(", array_value[t], ")");
        array_ref[t].push_back(array_value[t]);
        array_zen[t].push_back(zen::data::UInt32(array_value[t]));
        verify_arrays();
    }
}

void test_pop_front()
{
    for(size_t t = 0; t < 2; ++t )
    {
        if( !array_ref[t].empty() || ! array_zen[t].empty() )
        {
            ZEN_LOG("    ", test_number, "/", num_tests, " test_pop_front()");
            array_ref[t].pop_front();
            array_zen[t].pop_front();
            verify_arrays();
        }
    }
}

void test_pop_back()
{
    for(size_t t = 0; t < 2; ++t )
    {
        if( !array_ref[t].empty() || ! array_zen[t].empty() )
        {
            ZEN_LOG("    ", test_number, "/", num_tests, " test_pop_back()");
            array_ref[t].pop_back();
            array_zen[t].pop_back();
            verify_arrays();
        }
    }
}

void test_erase()
{
    for(size_t t = 0; t < 2; ++t )
    {
        if( !array_ref[t].empty() || ! array_zen[t].empty() )
        {
            uint32_t index = randomizer.get_ranged(array_zen[t].size());

            ZEN_LOG("    ", test_number, "/", num_tests, " test_erase(", index, ")");
    
            std::list<uint32_t>::iterator it = array_ref[t].begin();
            std::advance( it, index );

            array_ref[t].erase(it);
            array_zen[t].erase(index);
            verify_arrays();
        }
    }
}

void test_insert()
{
    for(size_t t = 0; t < 2; ++t )
    {
        if( !array_ref[t].empty() || ! array_zen[t].empty() )
        {
            array_value[t]++;
            uint32_t index = randomizer.get_ranged(array_zen[t].size());
            ZEN_LOG("    ", test_number, "/", num_tests, " test_insert(", index, ", ", array_value[t],")");
    
            std::list<uint32_t>::iterator it = array_ref[t].begin();
            std::advance( it, index );

            array_ref[t].insert(it, array_value[t]);
            array_zen[t].insert(index, array_value[t]);
            verify_arrays();
        }
    }
}

void test_container()
{
    verify_arrays();

    for(size_t t = 0; t < 2; ++t )
    {
        array_ref[t].clear();
        array_zen[t].clear();
    }

    num_tests = 1 + randomizer.get_ranged(2000);

    for ( test_number = 0; test_number < num_tests; ++test_number )
    {
        uint32_t test_function = randomizer.get_ranged(2);
        switch ( test_function )
        {
        default:
            break;
        case 0:
            test_push_front();
            break;
        case 1:
            test_push_back();
            break;
        case 2:
            test_pop_front();
            break;
        case 3:
            test_pop_back();
            break;
        case 4:
            test_erase();
            break;
        case 5:
            test_insert();
            break;
        case 6:
            test_bitstream_write();
            break;
        case 7:
            test_bitstream_poke();
            break;
        case 8:
            test_bitstream_read();
            break;
        case 9:
            test_bitstream_peek();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    verify_arrays();
}

void reset_test()
{
    // bitfield writer reset.
    uint32_t bitfield_capacity = randomizer.get_ranged(sizeof(bitfield_writer_data) << 3);
    bitfield_capacity = (bitfield_capacity / 32) * 32;
    bitfield_writer = zen::bitstream::Writer(bitfield_writer_data, bitfield_capacity);

    // bitfield reader reset.
    size_t bitfield_reader_size;
    randomizer.get_bits( bitfield_reader_data, bitfield_reader_size, (sizeof(bitfield_reader_data) << 3) );
    bitfield_reader = zen::bitstream::Reader(bitfield_reader_data, bitfield_reader_size);
 
    // reset data array.
    for(size_t t = 0; t < 2; ++t )
        array_value[t] = 0;
}

void main()
{
    while ( true )
    {
        reset_test();

        randomizer.seed(1000 + test_iteration);
        ZEN_LOG("test #", test_iteration,"...");
        test_container();
        test_iteration++;
    }
}