#pragma once

#include "zen/data/zen_data_integer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_default>
        Integer<TYPE, value_default>::Integer(TYPE value)
            : Value<TYPE>(value)
        {}

        template<typename TYPE, TYPE value_default>
        bool Integer<TYPE, value_default>::serialize_value(bitstream::Writer& out) const
        {
            zen::serializers::serialize_value(
                m_value,
                out,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Integer<TYPE>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_default>
        bool Integer<TYPE, value_default>::deserialize_value(bitstream::Reader& in)
        {
            zen::serializers::deserialize_value(
                m_value,
                in,
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Integer<TYPE>());
            return in.ok();
        }

        template<typename TYPE, TYPE value_default>
        bool Integer<TYPE, value_default>::serialize_delta(const Integer& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Integer<TYPE>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_default>
        bool Integer<TYPE, value_default>::deserialize_delta(const Integer& reference, bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Integer<TYPE>());
            return in.ok();
        }
    }
}
