#pragma once

#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_integer_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Writer& out);

        template<typename TYPE>
        bool deserialize_integer_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Reader& in);
    }
}

#include "zen/serializer/zen_serializer_integer_ranged.hpp"
