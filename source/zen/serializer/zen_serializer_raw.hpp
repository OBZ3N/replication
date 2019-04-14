#pragma once


#include "zen/serializer/zen_serializer_raw.h"
#include "zen/core/zen_core_endian.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_raw(const TYPE& value, bitstream::Writer& out)
        {
            TYPE n_value = core::convert_to_little_endian(value);

            return out.write(&n_value, sizeof(TYPE) << 3);
        }

        template<typename TYPE>
        bool deserialize_raw(TYPE& value, bitstream::Reader& in)
        {
            TYPE n_value;

            if (!in.read(&n_value, sizeof(TYPE) << 3))
                return false;

            value = core::convert_from_little_endian(n_value);

            return in.ok();
        }
    }
}
