#pragma once

#include "test/test_base.h"
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_utils.h"
#include "zen/data/zen_data_factory.h"

namespace test
{
    class Data : public Base
    {
    public:
        Data(const zen::data::Factory& factory, zen::debug::Randomizer& randomizer);

        void start_internal() override;
        bool update_internal() override;

    private:
        const zen::data::Factory& m_factory;
    };
}
