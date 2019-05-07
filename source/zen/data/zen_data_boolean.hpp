#pragma once

#include "zen/data/zen_data_boolean.h"
#include "zen/serializer/zen_serializer_boolean.h"

namespace zen
{
    namespace data
    {
        inline Boolean::Boolean()
            : m_value(false)
        {}

        inline Boolean::Boolean(bool value)
            : m_value(value)
        {}

        inline bool Boolean::set_value(bool value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

            return true;
        }

        inline bool Boolean::get_value() const
        {
            return m_value;
        }

        inline bool Boolean::operator == (const Boolean& rhs) const
        {
            if (m_value != rhs.m_value)
                return false;

            return true;
        }

        inline bool Boolean::operator != (const Boolean& rhs) const
        {
            return !((*this) == rhs);
        }

        inline Boolean& Boolean::operator = (const Boolean& rhs)
        {
            set_value(rhs.m_value);

            return *this;
        }

        inline bool Boolean::serialize_full(bitstream::Writer& out) const
        {
            zen::serializers::serialize_boolean(m_value, out);
            return out.ok();
        }

        inline bool Boolean::deserialize_full(bitstream::Reader& in)
        {
            bool value;
            zen::serializers::deserialize_boolean(value, in);
            if (!in.ok()) return false;

            set_value(value);

            return true;
        }

        inline bool Boolean::serialize_delta(const Boolean& reference, bitstream::Writer& out) const
        {
            if ((*this) == reference)
                return false;

            return serialize_full(out);
        }

        inline bool Boolean::deserialize_delta(const Boolean& reference, bitstream::Reader& in)
        {
            return deserialize_full(in);
        }
    }
}
