#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_default = 0>
        class Float: public Value<TYPE>
        {
        public:
            Float(TYPE value = value_default);

        protected:
            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const Float& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Float& reference, bitstream::Reader& in);
        };
    }
}

#include "zen/data/zen_data_float.hpp"
