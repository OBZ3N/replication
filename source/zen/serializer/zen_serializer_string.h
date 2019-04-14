#pragma once

#include <string>
#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        extern bool serialize_string(const std::string& value, bitstream::Writer& out);

        extern bool deserialize_string(std::string& value, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_string.hpp"
