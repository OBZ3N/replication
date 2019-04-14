#pragma once

#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        extern bool serialize_boolean(bool value, bitstream::Writer& out);
        extern bool deserialize_boolean(bool& value, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_boolean.hpp"
