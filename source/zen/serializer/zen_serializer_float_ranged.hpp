#pragma once

#include "zen/serializer/zen_serializer_float_ranged.h"
#include "zen/serializer/zen_serializer_log2.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_float_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Writer& out)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((num_bits > (TYPE)0), "num_bits(", num_bits, ") <= 0");
            ZEN_ASSERT((num_bits < 64), "num_bits(", num_bits, ") >= 64");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            uint64_t n_value;

            if (value <= value_min)
            {
                n_value = 0;
            }
            else
            {
                TYPE ratio = (value - value_min) / (value_max - value_min);

                n_value = uint64_t(ratio * TYPE(n_value_max));

                if (n_value >= n_value_max)
                {
                    n_value = n_value_max;
                }
            }

            n_value = core::convert_to_little_endian(n_value);

            return out.write(&n_value, num_bits);
        }

        template<typename TYPE>
        bool deserialize_float_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Reader& in)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((num_bits > (TYPE)0), "num_bits(", num_bits, ") <= 0");
            ZEN_ASSERT((num_bits < 64), "num_bits(", num_bits, ") >= 64");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            uint64_t n_value = 0;

            if (!in.read(&n_value, num_bits))
                return false;

            n_value = core::convert_from_little_endian(n_value);

            value = value_min + (TYPE(n_value) / TYPE(n_value_max));

            return in.ok();
        }
    }
}
