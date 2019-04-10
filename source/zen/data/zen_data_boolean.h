#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<bool value_default = false>
        class Boolean: public Value<bool>
        {
        public:
            Boolean(bool value = value_default);

        protected:
            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const Boolean& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Boolean& reference, bitstream::Reader& in);
        };
    }
}

#include "zen/data/zen_data_boolean.hpp"