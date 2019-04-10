#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<std::string value_default = "">
        class String: public Value<std::string>
        {
        public:
            String(const std::string& value = value_default);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const String& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const String& reference, bitstream::Reader& in);
        };
    }
}

#include "zen/data/zen_data_string.hpp"