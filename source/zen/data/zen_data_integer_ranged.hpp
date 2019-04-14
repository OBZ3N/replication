#pragma once

#include "zen/data/zen_data_integer_ranged.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        IntegerRanged<TYPE>::IntegerRanged(TYPE value, TYPE value_min, TYPE value_max, TYPE value_default)
            : m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
            , m_value_default(value_default)
        {}

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_value(bitstream::Writer& out) const
        {
            if (m_value == m_value_default)
            {
                return zen::serializers::serialize_boolean(true, out);
            }

            if (!zen::serializers::serialize_boolean(false, out))
                return false;

            return zen::serializers::serialize_integer_ranged(m_value, m_value_min, m_value_max, out);
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_value(bitstream::Reader& in)
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
                if (!zen::serializers::deserialize_integer_ranged(m_value, m_value_min, m_value_max, out))
                    return false;

                m_value = value;
                return in.ok();
            }
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_value(out);
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in)
        {
            return deserialize_value(in);
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            ZEN_ASSERT(value == m_value_default || (value >= m_value_min && value <= m_value_max),
                "value (", value, ") not valid for IntegerRanged(", "min=", m_value_min, ", max=", m_value_max, ", default=", m_value_default, ").");

            m_value = value;
            return true;
        }

        template<typename TYPE>
        TYPE IntegerRanged<TYPE>::get_value() const
        {
            return m_value;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator == (const IntegerRanged& rhs) const
        {
            return m_value == rhs.m_value;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator != (const IntegerRanged& rhs) const
        {
            return m_value != rhs.m_value;
        }
    }
}
