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
        FloatRanged<TYPE>::FloatRanged(Element* container)
            : Element(container)
            , m_value(0)
            , m_value_min(0)
            , m_value_max(0)
            , m_num_bits(0)
        {}

        template<typename TYPE>
        FloatRanged<TYPE>::FloatRanged(TYPE value, TYPE value_min, TYPE value_max, size_t num_bits, Element* container)
            : Element(container)
            , m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
            , m_num_bits(num_bits)
        {
            m_precision = calculate_precision();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            zen::serializers::serialize_raw(m_value, out);
            zen::serializers::serialize_raw(m_value_min, out);
            zen::serializers::serialize_raw(m_value_max, out);
            zen::serializers::serialize_raw(m_num_bits, out);
            return out.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            TYPE value;
            TYPE value_min;
            TYPE value_max;
            size_t num_bits;

            zen::serializers::deserialize_raw(value, in);
            zen::serializers::deserialize_raw(value_min, in);
            zen::serializers::deserialize_raw(value_max, in);
            zen::serializers::deserialize_raw(num_bits, in);
            
            if (!in.ok())
            {
                bool value_changed      =   (value      != m_value);
                bool attributes_changed =   (value_min  != m_value_min) ||
                                            (value_max  != m_value_max) ||
                                            (num_bits   != m_num_bits);

                set_touched(value_changed || attributes_changed);

                set_value_min(value_min);
                set_value_max(value_max);
                set_num_bits(num_bits);
                set_value(value);
            }
            return in.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const FloatRanged<TYPE>& reference = (const FloatRanged<TYPE>&) element_reference;

            bool value_changed      =   (m_value != reference.m_value);
            bool attributes_changed =   (m_value_min != reference.m_value_min) || 
                                        (m_value_max != reference.m_value_max) ||
                                        (m_num_bits != reference.m_num_bits);
            
            zen::serializers::serialize_boolean(value_changed, delta_bits);
            zen::serializers::serialize_boolean(attributes_changed, delta_bits);

            if (attributes_changed)
            {
                bool value_min_changed = (m_value_min != reference.m_value_min);
                bool value_max_changed = (m_value_max != reference.m_value_max);
                bool num_bits_changed  = (m_num_bits  != reference.m_num_bits);
                zen::serializers::serialize_boolean(value_min_changed, out);
                zen::serializers::serialize_boolean(value_max_changed, out);
                zen::serializers::serialize_boolean(num_bits_changed, out);

                if (value_min_changed)
                {
                    zen::serializers::serialize_raw(m_value_min, out);
                }

                if (value_max_changed)
                {
                    zen::serializers::serialize_raw(m_value_max, out);
                }

                if (num_bits_changed)
                {
                    zen::serializers::serialize_raw(m_num_bits, out);
                }
            }

            // value after attributes.
            if (value_changed)
            {
                zen::serializers::serialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, out);
            }

            return out.ok();
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            bool attributes_changed;
            bool value_changed;
            zen::serializers::deserialize_boolean(value_changed, delta_bits);
            zen::serializers::deserialize_boolean(attributes_changed, delta_bits);

            const FloatRanged<TYPE>& reference = (const FloatRanged<TYPE>&) element_reference;

            TYPE value = reference.m_value;
            TYPE value_min = reference.m_value_min;
            TYPE value_max = reference.m_value_max;
            size_t num_bits = reference.m_num_bits;

            if (attributes_changed)
            {
                bool value_min_changed;
                bool value_max_changed;
                bool num_bits_changed;
                zen::serializers::deserialize_boolean(value_min_changed, in);
                zen::serializers::deserialize_boolean(value_max_changed, in);
                zen::serializers::deserialize_boolean(num_bits_changed, in);
                if (!in.ok()) return false;

                if (value_min_changed)
                {
                    if (!zen::serializers::deserialize_raw(value_min, in))
                        return false;
                }
                
                if (value_max_changed)
                {
                    if (!zen::serializers::deserialize_raw(value_max, in))
                        return false;
                }

                if (num_bits_changed)
                {
                    if (!zen::serializers::deserialize_raw(num_bits, in))
                        return false;
                }
            }

            // value after attributes.
            if (value_changed)
            {
                if (!zen::serializers::deserialize_float_ranged(value, value_min, value_max, num_bits, in))
                    return false;
            }

            set_value(value);
            set_value_min(value_min);
            set_value_max(value_max);
            set_num_bits(num_bits);

            ZEN_ASSERT(m_value_max >= m_value_min, "value_max(", m_value_max, ") <= value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value >= m_value_min, "value(", m_value, ") < value_min(", m_value_min, ").");
            ZEN_ASSERT(m_value <= m_value_max, "value(", m_value, ") > value_max(", m_value_max, ").");

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

            m_precision = calculate_precision();

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

            m_precision = calculate_precision();

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

            m_precision = calculate_precision();

            set_touched(true);

            return true;
        }

        template<typename TYPE>
        size_t FloatRanged<TYPE>::get_num_bits() const
        {
            return m_num_bits;
        }

        template<typename TYPE>
        TYPE FloatRanged<TYPE>::calculate_precision() const
        {
            if (m_value_max <= m_value_min)
                return TYPE(0.0);

            if(m_num_bits == 0)
                return TYPE(0.0);

            TYPE precision = TYPE(m_value_max - m_value_min) / TYPE(1 << m_num_bits);

            return precision;
        }


        template<typename TYPE>
        bool FloatRanged<TYPE>::operator == (const FloatRanged<TYPE>& rhs) const
        {
            if (m_value < rhs.m_value - m_precision)
                return false;

            if (m_value > rhs.m_value + m_precision)
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
        bool FloatRanged<TYPE>::operator == (const Element& element_rhs) const
        {
            const FloatRanged<TYPE>& rhs = (const FloatRanged<TYPE>&) element_rhs;

            return (*this).operator==(rhs);
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::operator != (const Element& element_rhs) const
        {
            return !((*this) == element_rhs);
        }

        template<typename TYPE>
        FloatRanged<TYPE>& FloatRanged<TYPE>::operator = (const FloatRanged<TYPE>& rhs)
        {
            set_value(rhs.m_value);

            set_value_min(rhs.m_value_min);

            set_value_max(rhs.m_value_max);

            set_num_bits(rhs.m_num_bits);

            return *this;
        }

        template<typename TYPE>
        Element& FloatRanged<TYPE>::operator = (const Element& element_rhs)
        {
            const FloatRanged<TYPE>& rhs = (const FloatRanged<TYPE>&) element_rhs;

            return (*this).operator=(rhs);
        }

        template<typename TYPE>
        void FloatRanged<TYPE>::debug_randomize(debug::Randomizer& randomizer, float probability)
        {
            #undef min
            #undef max
            TYPE type_min   = -1.0E6;
            TYPE type_max   = 1.0E6;
            TYPE half_range = type_max / 2 - type_min / 2;
            TYPE min        = randomizer.get_float_ranged(type_min, half_range);
            TYPE max        = randomizer.get_float_ranged(min, (TYPE)(min + half_range));
            TYPE value      = randomizer.get_float_ranged(min, max);
            size_t num_bits = randomizer.get_integer_ranged(8, 24);

            if (randomizer.get_float_ranged(1.0f) < probability)
            {
                set_value_min(min);
            }

            if (randomizer.get_float_ranged(1.0f) < probability)
            {
                set_value_max(max);
            }

            if (randomizer.get_float_ranged(1.0f) < probability)
            {
                set_num_bits(num_bits);
            }

            if (randomizer.get_float_ranged(1.0f) < probability)
            {
                set_value(value);
            }
        }

        template<typename TYPE>
        bool FloatRanged<TYPE>::sanity_check() const
        {
            return true;
        }
    }
}
