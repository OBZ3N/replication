#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_default = 0>
        class Integer: public Value<TYPE>
        {
        public:
            Integer(TYPE value = value_default);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const Integer& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Integer& reference, bitstream::Reader& in);
        };

        typedef Integer<uint32_t> UInt32;
    }
}

#include "zen/data/zen_data_integer.hpp"