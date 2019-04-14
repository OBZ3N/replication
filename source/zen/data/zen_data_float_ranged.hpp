#pragma once

#include "zen/data/zen_data_float_ranged.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_float_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        FloatRanged<TYPE>::FloatRanged(TYPE value, TYPE value_min, TYPE value_max, size_t num_bits, TYPE value_default)
            : m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
            , m_value_default(value_default)
            , m_num_bits(num_bits)
        {}

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_value(bitstream::Writer& out) const
        {
            if (m_value == m_value_default)
            {
                return zen::serializers::serialize_boolean(true, out);
            }

            if (!zen::serializers::serialize_boolean(false, out))
                return false;

            return zen::serializers::serialize_float_ranged(m_value, m_value_min, m_value_max, num_bits, out);
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_value(bitstream::Reader& in)
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
                if (!zen::serializers::deserialize_float_ranged(m_value, m_value_min, m_value_max, num_bits, out))
                    return false;

                m_value = value;
                return in.ok();
            }
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_delta(const FloatRanged& reference, bitstream::Writer& out) const
        {
            if (m_value != reference.m_value)
                return false;

            return serialize_value(out);
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_delta(const FloatRanged& reference, bitstream::Reader& in)
        {
            return deserialize_value(in);
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            ZEN_ASSERT(value == m_value_default || (value >= m_value_min && value <= m_value_max),
                "value (", value, ") not valid for FloatRanged(", "min=", m_value_min,  ", max=", m_value_max, ", default=", m_value_default, ", num_bits=", m_num_bits, ").");

            m_value = value;
            return true;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::get_value() const
        {
            return m_value;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::operator == (const FloatRanged& rhs) const
        {
            return m_value == rhs.m_value;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::operator != (const FloatRanged& rhs) const
        {
            return m_value != rhs.m_value;
        }
    }
}
