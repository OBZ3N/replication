#pragma once

#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool serialize_integer_ranged(const TYPE& value, bitstream::Writer& out);

        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool deserialize_integer_ranged(TYPE& value, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_integer_ranged.hpp"
