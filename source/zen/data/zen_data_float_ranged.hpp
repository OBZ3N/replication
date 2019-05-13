#pragma once

#include "zen/data/zen_data_float_ranged.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_float_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        FloatRanged<TYPE>::FloatRanged()
            : m_value(0)
            , m_value_min(0)
            , m_value_max(0)
            , m_num_bits(0)
        {}

        template<typename TYPE>
        FloatRanged<TYPE>::FloatRanged(TYPE value, TYPE value_min, TYPE value_max, size_t num_bits)
            : m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
            , m_num_bits(num_bits)
        {}

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            if (!zen::serializers::serialize_raw(m_value, out))
                return false;

            if (!zen::serializers::serialize_raw(m_value_min, out))
                return false;

            if (!zen::serializers::serialize_raw(m_value_max, out))
                return false;

            if (!zen::serializers::serialize_raw(m_num_bits, out))
                return false;

            return out.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            TYPE value;
            TYPE value_min;
            TYPE value_max;
            size_t num_bits;

            if (!zen::serializers::deserialize_raw(value, in))
                return false;

            if (!zen::serializers::deserialize_raw(value_min, in))
                return false;

            if (!zen::serializers::deserialize_raw(value_max, in))
                return false;

            if (!zen::serializers::deserialize_raw(num_bits, in))
                return false;

            set_value(value);
            set_value_min(value_min);
            set_value_max(value_max);
            set_num_bits(num_bits);

            return in.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const FloatRanged<TYPE>& reference = (const FloatRanged<TYPE>&) element_reference;

            bool attributes_changed =   (m_value_min != reference.m_value_min) || 
                                        (m_value_max != reference.m_value_max) ||
                                        (m_num_bits != reference.m_num_bits);

            if (!zen::serializers::serialize_boolean(attributes_changed, delta_bits))
                return false;

            if (attributes_changed)
            {
                bool value_min_changed = (m_value_min != reference.m_value_min);
                bool value_max_changed = (m_value_max != reference.m_value_max);
                bool num_bits_changed  = (m_num_bits  != reference.m_num_bits);

                if (!zen::serializers::serialize_boolean(value_min_changed, delta_bits))
                    return false;

                if (!zen::serializers::serialize_boolean(value_max_changed, delta_bits))
                    return false;

                if (!zen::serializers::serialize_boolean(num_bits_changed, delta_bits))
                    return false;

                if (value_min_changed)
                {
                    if (!zen::serializers::serialize_raw(m_value_min, out))
                        return false;
                }

                if (value_max_changed)
                {
                    if(!zen::serializers::serialize_raw(m_value_max, out))
                        return false;
                }

                if (num_bits_changed)
                {
                    if(!zen::serializers::serialize_raw(m_num_bits, out))
                        return false;
                }
            }

            bool value_changed = (m_value != reference.m_value);

            if (!zen::serializers::serialize_boolean(value_changed, delta_bits))
                return false;

            if (value_changed)
            {
                if (!zen::serializers::serialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, out))
                    return false;
            }

            return out.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const FloatRanged<TYPE>& reference = (const FloatRanged<TYPE>&) element_reference;

            bool attributes_changed;
            if (!zen::serializers::deserialize_boolean(attributes_changed, delta_bits))
                return false;

            if (attributes_changed)
            {
                bool value_min_changed;
                if (!zen::serializers::deserialize_boolean(value_min_changed, in))
                    return false;

                bool value_max_changed;
                if (!zen::serializers::deserialize_boolean(value_max_changed, in))
                    return false;

                bool num_bits_changed;
                if (!zen::serializers::deserialize_boolean(num_bits_changed, in))
                    return false;

                if (value_min_changed)
                {
                    if (!zen::serializers::deserialize_raw(m_value_min, in))
                        return false;
                }

                if (value_max_changed)
                {
                    if (!zen::serializers::deserialize_raw(m_value_max, in))
                        return false;
                }

                if (num_bits_changed)
                {
                    if (!zen::serializers::deserialize_raw(m_num_bits, in))
                        return false;
                }
            }

            bool value_changed;
            if (!zen::serializers::deserialize_boolean(value_changed, delta_bits))
                return false;

            if (value_changed)
            {
                if (!zen::serializers::deserialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, in))
                    return false;
            }

            return in.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::set_value(TYPE value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::get_value() const
        {
            return m_value;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::set_value_min(TYPE value_min)
        {
            if (m_value_min == value_min)
                return false;

            m_value_min = value_min;

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::get_value_min() const
        {
            return m_value_min;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::set_value_max(TYPE value_max)
        {
            if (m_value_max == value_max)
                return false;

            m_value_max = value_max;

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::get_value_max() const
        {
            return m_value_max;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::set_num_bits(size_t num_bits)
        {
            if (m_num_bits == num_bits)
                return false;

            m_num_bits = num_bits;

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::get_num_bits() const
        {
            return m_num_bits;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::operator == (const Element& element_rhs_rhs) const
        {
            const FloatRanged<TYPE>& rhs = (const FloatRanged<TYPE>&) element_rhs;

            if (m_value != rhs.m_value)
                return false;

            if (m_value_min != rhs.m_value_min)
                return false;

            if (m_value_max != rhs.m_value_max)
                return false;

            if (m_num_bits != rhs.m_num_bits)
                return false;

            return true;
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::operator != (const Element& element_rhs) const
        {
            const FloatRanged<TYPE>& rhs = (const FloatRanged<TYPE>&) element_rhs;

            return !((*this) == rhs);
        }

        template<typename TYPE>
        Element& FloatRanged<TYPE>::operator = (const Element& element_rhs)
        {
            const FloatRanged<TYPE>& rhs = (const FloatRanged<TYPE>&) element_rhs;

            set_value(rhs.m_value);

            set_value_min(rhs.m_value_min);

            set_value_max(rhs.m_value_max);

            set_num_bits(rhs.m_num_bits);

            return *this;
        }
    }
}
