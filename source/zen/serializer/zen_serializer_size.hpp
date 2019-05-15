#pragma once

#include "zen/serializer/zen_serializer_size.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/core/zen_core_endian.h"

namespace zen
{
    namespace serializers
    {
        constexpr uint64_t serialize_size_bitcount = uint64_t(7);
        constexpr uint64_t serialize_size_bitmask = uint64_t((1 << serialize_size_bitcount)-1);

        template<typename TYPE>
        bool serialize_size(const TYPE& value, bitstream::Writer& out)
        {
            ZEN_ASSERT(value >= 0, "value (", value, ") < 0.");

            do
            {
                uint64_t chunk = core::convert_to_little_endian(uint64_t(value) & serialize_size_bitmask);

                value >>= serialize_size_bitcount;

                uint8_t remaining = (value != 0) ? 1 : 0;

                out.write(&chunk, serialize_size_bitcount);

                out.write(&remaining, 1);
            }
            while (value != 0);

            return out.ok();
        }

        template<typename TYPE>
        bool deserialize_size(TYPE& value, bitstream::Reader& in)
        {
            value = 0;

            uint64_t shift = 0;

            uint64_t chunk;

            uint8_t remaining;

            do
            {
                in.read(&chunk, serialize_size_bitcount);

                in.read(&remaining, 1);

                if (!in.ok())
                    return false;

                value |= TYPE(core::convert_from_little_endian(chunk) << shift);

                shift += serialize_size_bitcount;
            }
            while (remaining != 0);

            return in.ok();
        }
    }
}
