
#include <algorithm>
#include <thread>
#include <chrono>
#include <cinttypes>
#include <string>
#include <vector>
#include <unordered_map>

#include "zen/debug/zen_debug_assert.h"
#include "zen/data/zen_data_vector.h"
#include "zen/data/zen_data_raw.h"
#include "zen/data/zen_data_vector.h"
#include "zen/bitstream/zen_bitstream_utils.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

#include "test/test_bitstream.h"
#include "test/test_vector.h"
#include "test/test_vector_serialization.h"


void main()
{
    zen::debug::Randomizer randomizer;

    std::vector<test::Base*> tests;

    //tests.push_back(new test::Bitstream(randomizer));
    //tests.push_back(new test::Vector<uint32_t, zen::data::Raw<uint32_t>>(randomizer));
    tests.push_back(new test::VectorSerialization<uint32_t, zen::data::Raw<uint32_t>>(randomizer));

    uint32_t test_iteration = 0;

    while ( true )
    {
        test_iteration++;

        randomizer.seed(1000 + test_iteration);

        ZEN_LOG("test #", test_iteration,"...");

        uint32_t test = randomizer.get_integer_ranged(tests.size());

        uint32_t num_iterations = randomizer.get_integer_ranged(10, 100);

        tests[test]->start(test_iteration, num_iterations);

        while (tests[test]->get_state() == test::Base::State::Running)
        {
            tests[test]->update();

            Sleep(1);
        }

        Sleep(1);
    }
}