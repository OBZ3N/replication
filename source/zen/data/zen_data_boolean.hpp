#pragma once

#include "zen/data/zen_data_boolean.h"
#include "zen/serializer/zen_serializer_boolean.h"

namespace zen
{
    namespace data
    {
        inline Boolean::Boolean(bool value, bool value_default)
            : m_value(value)
            , m_value_default(value_default)
        {}

        inline bool Boolean::serialize_value(bitstream::Writer& out) const
        {
            return zen::serializers::serialize_boolean(m_value, out);
        }

        inline bool Boolean::deserialize_value(bitstream::Reader& in)
        {
            bool value;
            if (!zen::serializers::deserialize_boolean(m_value, in))
                return false;

            m_value = value;
            return in.ok();
        }

        inline bool Boolean::serialize_delta(const Boolean& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_value(out);
        }

        inline bool Boolean::deserialize_delta(const Boolean& reference, bitstream::Reader& in)
        {
            return deserialize_value(in);
        }

        inline bool Boolean::set_value(bool value)
        {
            if (m_value == value)
                return false;

            m_value = value;
            return true;
        }

        inline bool Boolean::get_value() const
        {
            return m_value;
        }

        inline bool Boolean::operator == (const Boolean& rhs) const
        {
            return m_value == rhs.m_value;
        }

        inline bool Boolean::operator != (const Boolean& rhs) const
        {
            return m_value != rhs.m_value;
        }
    }
}
