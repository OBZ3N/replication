#pragma once

#include "zen/serializer/zen_serializer_boolean.h"

namespace zen
{
    namespace serializers
    {
        inline bool serialize_boolean(bool value, bitstream::Writer& out)
        {
            uint8_t flag = (value) ? 1 : 0;

            return out.write(&flag, 1);
        }

        inline bool deserialize_boolean(bool& value, bitstream::Reader& in)
        {
            uint8_t flag = 0;

            if (!in.read(&flag, 1))
                return false;

            value = (flag != 0);

            return in.ok();
        }
    }
}
