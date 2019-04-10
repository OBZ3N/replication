#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default=0>
        class FloatRanged: public Value<TYPE>
        {
        public:
            FloatRanged(TYPE value = value_default);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const FloatRanged& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const FloatRanged& reference, bitstream::Reader& in);
        };
    }
}

#include "zen/data/zen_data_float_ranged.hpp"