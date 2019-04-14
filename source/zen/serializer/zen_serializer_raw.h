#pragma once

#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_raw(const TYPE& value, bitstream::Writer& out);

        template<typename TYPE>
        bool deserialize_raw(TYPE& value, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_raw.hpp"
