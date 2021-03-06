#pragma once

#include "zen/data/zen_data_string.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_string.h"

namespace zen
{
    namespace data
    {
        inline String::String(const std::string& value, const std::string& value_default)
            : m_value(value)
            , m_value_default(value)
        {}

        inline String::String(const char* value, const char* value_default)
            : m_value(value)
            , m_value_default(value)
        {}

        inline bool String::serialize_value(bitstream::Writer& out) const
        {
            if (m_value == m_value_default)
            {
                return zen::serializers::serialize_boolean(true, out);
            }

            if (!zen::serializers::serialize_boolean(false, out))
                return false;

            return zen::serializers::serialize_string(m_value, out);
        }

        inline bool String::deserialize_value(bitstream::Reader& in)
        {
            bool is_default;
            if (!zen::serializers::deserialize_boolean(is_default, in))
                return false;

            if (is_default)
            {
                m_value = m_value_default;
                return in.ok();
            }
            else
            {
                std::string value;
                if (!zen::serializers::deserialize_string(value, in))
                    return false;

                m_value = value;
                return in.ok();
            }
        }

        inline bool String::serialize_delta(const String& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_value(out);
        }

        inline bool String::deserialize_delta(const String& reference, bitstream::Reader& in)
        {
            return deserialize_value(in);
        }

        inline bool String::set_value(const std::string& value)
        {
            if (m_value == value)
                return false;

            m_value = value;
            return true;
        }

        inline const std::string& String::get_value() const
        {
            return m_value;
        }

        inline bool String::operator == (const String& rhs) const
        {
            return m_value == rhs.m_value;
        }

        inline bool String::operator != (const String& rhs) const
        {
            return m_value != rhs.m_value;
        }
    }
}