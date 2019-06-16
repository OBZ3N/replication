#pragma once

#include "test/test_base.h"
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_utils.h"

namespace test
{
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    class VectorSerialization : public Base
    {
    public:
        VectorSerialization(zen::debug::Randomizer& randomizer);

        void start_internal() override;
        bool update_internal() override;

    public:
        bool validate();

        zen::data::Vector<ZEN_TYPE> m_reference;
        zen::data::Vector<ZEN_TYPE> m_input;
        zen::data::Vector<ZEN_TYPE> m_output;

        uint32_t m_data[10000];
    };
}

#include "test/test_vector_serialization.hpp"
