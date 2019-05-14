#pragma once

#include "zen/serializer/zen_serializer_size.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/core/zen_core_endian.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_size(const TYPE& value, bitstream::Writer& out)
        {
            ZEN_ASSERT(value >= 0, "value (", value, ") < 0.");

            do
            {
                uint8_t chunk = uint8_t(value & 255);

                value >>= 8;

                bool remaining = (value != 0);

                serialize_raw(chunk, out);

                serialize_boolean(remaining, out);
            }
            while (value != 0);

            return out.ok();
        }

        template<typename TYPE>
        bool deserialize_size(TYPE& value, bitstream::Reader& in)
        {
            value = 0;

            size_t shift = 0;

            uint8_t chunk;

            bool remaining;

            do
            {
                deserialize_raw(chunk, in);

                deserialize_boolean(remaining, in);

                if (!in.ok())
                    return false;

                value |= ((TYPE)chunk << (TYPE)shift);

                shift += 8;
            }
            while (remaining);

            return in.ok();
        }
    }
}
