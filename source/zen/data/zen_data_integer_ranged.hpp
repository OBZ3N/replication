#pragma once

#include "zen/data/zen_data_integer_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default>
        IntegerRanged<TYPE, value_min, value_max, value_default>::IntegerRanged(TYPE value)
            : Value<TYPE>(value)
        {}

        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default>
        bool IntegerRanged<TYPE, value_min, value_max, value_default>::serialize_value(bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                out,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::IntegerRanged<TYPE, value_min, value_max>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default>
        bool IntegerRanged<TYPE, value_min, value_max, value_default>::deserialize_value(bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                in,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::IntegerRanged<TYPE, value_min, value_max>());
            return in.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default>
        bool IntegerRanged<TYPE, value_min, value_max, value_default>::serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::IntegerRanged<TYPE, value_min, value_max>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, TYPE value_default>
        bool IntegerRanged<TYPE, value_min, value_max, value_default>::deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::IntegerRanged<TYPE, value_min, value_max>());
            return in.ok();
        }
    }
}
