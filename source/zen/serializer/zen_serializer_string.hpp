#pragma once

#include "zen/serializer/zen_serializer_string.h"

namespace zen
{
    namespace serializers
    {
        inline bool serialize_string(const std::string& value, bitstream::Writer& out)
        {
            size_t length = value.length();

            const char* buffer = &(value[0]);

            return out.write(buffer, (length << 3));
        }

        inline bool deserialize_string(std::string& value, bitstream::Reader& in)
        {
            std::stringstream n_value;

            char character;
            do
            {
                if (!in.read(&character, 8))
                    return false;

                n_value << character;
            } while (character != '\0');

            value = n_value.str();

            return in.ok();
        }
    }
}
