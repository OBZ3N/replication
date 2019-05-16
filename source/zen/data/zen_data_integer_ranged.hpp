#pragma once

#include "zen/data/zen_data_integer_ranged.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_log2.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        IntegerRanged<TYPE>::IntegerRanged()
            : m_value(0)
            , m_value_min(0)
            , m_value_max(0)
            , m_num_bits(0)
        {}

        template<typename TYPE>
        IntegerRanged<TYPE>::IntegerRanged(TYPE value, TYPE value_min, TYPE value_max)
            : m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
            , m_num_bits(zen::serializers::number_of_bits_required(value_max - value_min))
        {
            ZEN_ASSERT(m_value_max > m_value_min, "value_max(", m_value_max, ") <= value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value >= m_value_min, "value(", m_value, ") < value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value <= m_value_max, "value(", m_value, ") > value_max(", m_value_max, ").");
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            zen::serializers::serialize_raw(m_value, out);
            zen::serializers::serialize_raw(m_value_min, out);
            zen::serializers::serialize_raw(m_value_max, out);

            return out.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            TYPE value;
            TYPE value_min;
            TYPE value_max;
            zen::serializers::deserialize_raw(value, in);
            zen::serializers::deserialize_raw(value_min, in);
            zen::serializers::deserialize_raw(value_max, in);

            if (in.ok())
            {
                set_value(value);
                set_value_min(value_min);
                set_value_max(value_max);
            }
            return in.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const IntegerRanged<TYPE>& reference = (const IntegerRanged<TYPE>&)element_reference;

            bool value_changed      = (m_value != reference.m_value);
            bool attributes_changed = (m_value_min != reference.m_value_min) ||
                                      (m_value_max != reference.m_value_max);

            zen::serializers::serialize_boolean(value_changed, delta_bits);
            zen::serializers::serialize_boolean(attributes_changed, delta_bits);

            if (attributes_changed)
            {
                bool value_min_changed = (m_value_min != reference.m_value_min);
                bool value_max_changed = (m_value_max != reference.m_value_max);

                zen::serializers::serialize_boolean(value_min_changed, out);
                zen::serializers::serialize_boolean(value_max_changed, out);
                
                if (value_min_changed)
                {
                    zen::serializers::serialize_raw(m_value_min, out);
                }

                if (value_max_changed)
                {
                    zen::serializers::serialize_raw(m_value_max, out);
                }
            }

            if (value_changed)
            {
                zen::serializers::serialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, out);
            }

            return out.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const IntegerRanged<TYPE>& reference = (const IntegerRanged<TYPE>&)element_reference;

            bool attributes_changed;
            bool value_changed;

            zen::serializers::deserialize_boolean(value_changed, delta_bits);
            zen::serializers::deserialize_boolean(attributes_changed, delta_bits);

            if (attributes_changed)
            {
                bool value_min_changed;
                bool value_max_changed;
                zen::serializers::deserialize_boolean(value_min_changed, in);
                zen::serializers::deserialize_boolean(value_max_changed, in);

                if (value_min_changed)
                {
                    TYPE value_min;
                    zen::serializers::deserialize_raw(value_min, in);
                    if (in.ok())
                        set_value_min(value_min);
                }

                if (value_max_changed)
                {
                    TYPE value_max;
                    zen::serializers::deserialize_raw(value_max, in);
                    if (in.ok())
                        set_value_max(value_max);
                }

                m_num_bits = zen::serializers::num_bits_required(m_value_min, m_value_max);
            }

            if (value_changed)
            {
                TYPE value;
                zen::serializers::deserialize_float_ranged(value, m_value_min, m_value_max, m_num_bits, in);
                if (in.ok())
                    set_value(value);
            }

            ZEN_ASSERT(m_value_max > m_value_min, "value_max(", m_value_max, ") <= value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value >= m_value_min, "value(", m_value, ") < value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value <= m_value_max, "value(", m_value, ") > value_max(", m_value_max, ").");

            return in.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        TYPE IntegerRanged<TYPE>::get_value() const
        {
            return m_value;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::set_value_min(TYPE value_min)
        {
            if (m_value_min == value_min)
                return false;

            m_value_min = value_min;

            set_touched(true);

            if (m_value_min < m_value_max)
            {
                m_num_bits = zen::serializers::num_bits_required(m_value_min, m_value_max);
            }

            return true;
        }

        template<typename TYPE>
        TYPE IntegerRanged<TYPE>::get_value_min() const
        {
            return m_value_min;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::set_value_max(TYPE value_max)
        {
            if (m_value_max == value_max)
                return false;

            m_value_max = value_max;

            set_touched(true);

            if (m_value_max > m_value_min)
            {
                m_num_bits = zen::serializers::num_bits_required(m_value_min, m_value_max);
            }

            return true;
        }

        template<typename TYPE>
        TYPE IntegerRanged<TYPE>::get_value_max() const
        {
            return m_value_max;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator == (const Element& element_rhs) const
        {
            const IntegerRanged<TYPE>& rhs = (const IntegerRanged<TYPE>&) element_rhs;

            if (m_value != rhs.m_value)
                return false;

            if (m_value_min != rhs.m_value_min)
                return false;

            if (m_value_max != rhs.m_value_max)
                return false;

            return true;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator != (const Element& rhs) const
        {
            const IntegerRanged<TYPE>& rhs = (const IntegerRanged<TYPE>&)element_rhs;

            return !((*this) == rhs);
        }

        template<typename TYPE>
        Element& IntegerRanged<TYPE>::operator = (const Element& element_rhs)
        {
            const IntegerRanged<TYPE>& rhs = (const IntegerRanged<TYPE>&) element_rhs;

            set_value(rhs.m_value);
            set_value_min(rhs.m_value_min);
            set_value_max(rhs.m_value_max);
            return *this;
        }
    }
}
