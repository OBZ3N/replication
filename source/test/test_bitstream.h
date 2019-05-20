#pragma once

#include "test/test_base.h"
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_utils.h"

namespace test
{
    class Bitstream : public Base
    {
    public:
        Bitstream(zen::debug::Randomizer& randomizer);

        void start_internal() override;
        bool update_internal() override;

    public:
        bool write();
        bool read();
        bool peek();
        bool poke();

        uint32_t m_data[1000];
        uint32_t m_temp[1000];
    };

}
