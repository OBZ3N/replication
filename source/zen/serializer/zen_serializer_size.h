#pragma once

#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        // decompose values into non-zero bytes.
        template<typename TYPE>
        bool serialize_size(TYPE value, bitstream::Writer& out);

        template<typename TYPE>
        bool deserialize_size(TYPE& value, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_size.hpp"
