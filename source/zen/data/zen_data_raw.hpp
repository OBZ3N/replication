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
        Element& Raw<TYPE>::operator = (const Element& element_rhs)
        {
            const Raw<TYPE>& rhs = (const Raw<TYPE>&) element_rhs;

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
            zen::serializers::deserialize_raw(value, in);

            if(in.ok())
                set_value(value);

            return in.ok();
        }

        template<typename TYPE>
        bool Raw<TYPE>::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Raw<TYPE>& reference = (const Raw<TYPE>&) element_reference;

            bool value_changed = (m_value != reference.m_value);
            serializers::serialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                zen::serializers::serialize_raw(m_value, out);
            }

            return out.ok();
        }

        template<typename TYPE>
        bool Raw<TYPE>::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Raw<TYPE>& reference = (const Raw<TYPE>&) element_reference;

            bool value_changed;
            serializers::deserialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                TYPE value;
                zen::serializers::deserialize_raw(value, in);

                if (in.ok())
                    set_value(value);
            }
            return in.ok();
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
        bool Raw<TYPE>::operator == (const Element& element_rhs) const
        {
            const Raw<TYPE>& rhs = (const Raw<TYPE>&) element_rhs;

            return m_value == rhs.m_value;
        }

        template<typename TYPE>
        bool Raw<TYPE>::operator != (const Element& element_rhs) const
        {
            const Raw<TYPE>& rhs = (const Raw<TYPE>&) element_rhs;

            return m_value != rhs.m_value;
        }
    }
}
