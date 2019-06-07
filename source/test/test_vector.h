#pragma once

#include "test/test_base.h"
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_utils.h"

namespace test
{
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    class Vector : public Base
    {
    public:
        Vector(zen::debug::Randomizer& randomizer);

        void start_internal() override;
        bool update_internal() override;

    public:
        bool push_front();
        bool pop_front();
        bool push_back();
        bool pop_back();
        bool erase();
        bool insert();
        bool validate();

        GENERIC_TYPE m_value_iterator;
        std::vector<GENERIC_TYPE> m_generic_array;
        zen::data::Vector<ZEN_TYPE> m_zen_array;
    };
}

#include "test/test_vector.hpp"
