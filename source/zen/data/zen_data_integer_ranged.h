#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default=0>
        class IntegerRanged: public Value<TYPE>
        {
        public:
            IntegerRanged(TYPE value = value_default);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in);
        };
    }
}

#include "zen/data/zen_data_integer_ranged.hpp"