#pragma once

#include "zen/data/zen_data_raw.h"
#include "zen/serializer/zen_serializer_raw.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Raw<TYPE>::Raw()
            : m_value(0)
        {}

        template<typename TYPE>
        Raw<TYPE>::Raw(TYPE value)
            : m_value(value)
        {}

        template<typename TYPE>
        Raw<TYPE>& Raw<TYPE>::operator = (const Raw<TYPE>& rhs)
        {
            set_value(rhs.m_value);

            return *this;
        }

        template<typename TYPE>
        bool Raw<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            return zen::serializers::serialize_raw(m_value, out);
        }

        template<typename TYPE>
        bool Raw<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            TYPE value;
            if (!zen::serializers::deserialize_raw(value, in))
                return false;

            set_value(value);

            return in.ok();
        }

        template<typename TYPE>
        bool Raw<TYPE>::serialize_delta(const Raw<TYPE>& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_full(out);
        }

        template<typename TYPE>
        bool Raw<TYPE>::deserialize_delta(const Raw& reference, bitstream::Reader& in)
        {
            return deserialize_full(in);
        }

        template<typename TYPE>
        bool Raw<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

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
