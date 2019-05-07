#pragma once

#include "zen/data/zen_data_string.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_string.h"

namespace zen
{
    namespace data
    {
        inline String::String()
        {}

        inline String::String(const std::string& value)
            : m_value(value)
        {}

        inline String::String(const char* value)
            : m_value(value)
        {}

        inline String& String::operator = (const String& rhs)
        {
            set_value(rhs.m_value);

            return *this;
        }
        
        inline bool String::serialize_full(bitstream::Writer& out) const
        {
            return zen::serializers::serialize_string(m_value, out);
        }

        inline bool String::deserialize_full(bitstream::Reader& in)
        {
            std::string value;
            if (!zen::serializers::deserialize_string(value, in))
                return false;

            set_value(value);

            return in.ok();
        }

        inline bool String::serialize_delta(const String& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_full(out);
        }

        inline bool String::deserialize_delta(const String& reference, bitstream::Reader& in)
        {
            return deserialize_full(in);
        }

        inline bool String::set_value(const std::string& value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

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