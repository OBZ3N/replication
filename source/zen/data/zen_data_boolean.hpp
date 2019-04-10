#pragma once

#include "zen/data/zen_data_boolean.h"

namespace zen
{
    namespace data
    {
        template<bool value_default>
        Boolean<value_default>::Boolean(bool value = value_default)
            : Value<bool>(value)
        {}

        template<bool value_default>
        bool Boolean<value_default>::serialize_value(bitstream::Writer& out) const
        {
            zen::serializers::serialize_value(
                m_value,
                out,
                zen::serializers::Boolean());
            return out.ok();
        }

        template<bool value_default>
        bool Boolean<value_default>::deserialize_value(bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                in,
                zen::serializers::Boolean());
            return in.ok();
        }

        template<bool value_default>
        bool Boolean<value_default>::serialize_delta(const Boolean& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<bool>(),
                zen::serializers::Boolean());
            return out.ok();
        }

        template<bool value_default>
        bool Boolean<value_default>::deserialize_delta(const Boolean& reference, bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<bool>(),
                zen::serializers::Boolean());
            return in.ok();
        }
    }
}
