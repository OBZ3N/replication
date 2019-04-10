#pragma once

#include "zen/data/zen_data_string.h"

namespace zen
{
    namespace data
    {
        template<std::string value_default>
        String<value_default>::String(const std::string& value)
            : Value<std::string>(value)
        {
        }

        template<std::string value_default>
        bool String<value_default>::serialize_value(bitstream::Writer& out) const
        {
            zen::serializers::serialize_value(
                m_value,
                out,
                zen::serializers::Default<std::string, value_default>(),
                zen::serializers::String());
            return out.ok();
        }

        template<std::string value_default>
        bool String<value_default>::deserialize_value(bitstream::Reader& in)
        {
            zen::serializers::deserialize_value(
                m_value,
                in,
                zen::serializers::Default<std::string, value_default>(),
                zen::serializers::String());
            return in.ok();
        }

        template<std::string value_default>
        bool String<value_default>::serialize_delta(const String& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<std::string>(),
                zen::serializers::Default<std::string, value_default>(),
                zen::serializers::String());
            return out.ok();
        }

        template<std::string value_default>
        bool String<value_default>::deserialize_delta(const String& reference, bitstream::Reader& in)
        {
            zen::serializers::deserialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<std::string>(),
                zen::serializers::Default<std::string, value_default>(),
                zen::serializers::String());
            return in.ok();
        }
    }
}

#include "zen/data/zen_data_float_integer_string.hpp"