#pragma once

#include "zen/core/zen_core_endian.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"
#include "zen/serializer/zen_serializer_log2.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_integer_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Writer& out)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            const uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            TYPE n_value;

            if (value <= value_min)
            {
                n_value = core::convert_to_little_endian(0);
            }
            else if (value >= value_max)
            {
                n_value = core::convert_to_little_endian(value - value_min);

                if (n_value > n_value_max)
                {
                    n_value = n_value_max;
                }
            }

            return out.write(&n_value, num_bits);
        }

        template<typename TYPE>
        bool deserialize_integer_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Reader& in)
        {
            TYPE n_value = 0;
            if (!in.read(&n_value, num_bits))
                return false;

            value = value_min + core::convert_from_little_endian(n_value);

            return in.ok();
        }
    }
}
