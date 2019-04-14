#pragma once

#include "zen/data/zen_data_raw.h"
#include "zen/serializer/zen_serializer_raw.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Raw<TYPE>::Raw(TYPE value, TYPE value_default)
            : m_value(value)
            , m_value_default(value_default)
        {}

        template<typename TYPE>
        bool Raw<TYPE>::serialize_value(bitstream::Writer& out) const
        {
            if (m_value == m_value_default)
            {
                return zen::serializers::serialize_boolean(true, out);
            }

            if (!zen::serializers::serialize_boolean(false, out))
                return false;

            return zen::serializers::serialize_raw(m_value, out);
        }

        template<typename TYPE>
        bool Raw<TYPE>::deserialize_value(bitstream::Reader& in)
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
                TYPE value;
                if (!zen::serializers::deserialize_raw(value, in))
                    return false;

                m_value = value;
                return in.ok();
            }
        }

        template<typename TYPE>
        bool Raw<TYPE>::serialize_delta(const Raw& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_value(out);
        }

        template<typename TYPE>
        bool Raw<TYPE>::deserialize_delta(const Raw& reference, bitstream::Reader& in)
        {
            return deserialize_value(in);
        }

        template<typename TYPE>
        bool Raw<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            m_value = value;
            return true;
        }

        template<typename TYPE>
        TYPE Raw<TYPE>::get_value() const
        {
            return m_value;
        }

        template<typename TYPE>
        bool Raw<TYPE>::operator == (const Raw& rhs) const
        {
            return m_value == rhs.m_value;
        }

        template<typename TYPE>
        bool Raw<TYPE>::operator != (const Raw& rhs) const
        {
            return m_value != rhs.m_value;
        }
    }
}
