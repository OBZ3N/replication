#pragma once

#include "zen/data/zen_data_float_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default>
        FloatRanged<TYPE, value_min, value_max, num_bits, value_default>::FloatRanged(TYPE value)
            : Value<TYPE>(value)
        {}

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default>
        bool FloatRanged<TYPE, value_min, value_max, num_bits, value_default>::serialize_value(bitstream::Writer& out) const
        {
            zen::serializers::serialize_value(
                m_value,
                out,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::FloatRanged<TYPE, value_min, value_max, num_bits>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default>
        bool FloatRanged<TYPE, value_min, value_max, num_bits, value_default>::deserialize_value(bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference->m_value,
                in,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::FloatRanged<TYPE, value_min, value_max, num_bits>());
            return in.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default>
        bool FloatRanged<TYPE, value_min, value_max, num_bits, value_default>::serialize_delta(const FloatRanged& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::FloatRanged<TYPE, value_min, value_max, num_bits>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default>
        bool FloatRanged<TYPE, value_min, value_max, num_bits, value_default>::deserialize_delta(const FloatRanged& reference, bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::FloatRanged<TYPE, value_min, value_max, num_bits>());
            return in.ok();
        }
    }
}
