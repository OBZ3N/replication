#pragma once

#include "zen/data/zen_data_integer_ranged.h"
#include "zen/serializer/zen_serializer_boolean.h"
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
        {}

        template<typename TYPE>
        IntegerRanged<TYPE>::IntegerRanged(TYPE value, TYPE value_min, TYPE value_max)
            : m_value(value)
            , m_value_min(value_min)
            , m_value_max(value_max)
        {}

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            if (!zen::serializers::serialize_raw(m_value, out))
                return false;

            if (!zen::serializers::serialize_raw(m_value_min, out))
                return false;

            if (!zen::serializers::serialize_raw(m_value_max, out))
                return false;

            return out.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            TYPE value;
            TYPE value_min;
            TYPE value_max;

            if (!zen::serializers::deserialize_raw(value, in))
                return false;

            if (!zen::serializers::deserialize_raw(value_min, in))
                return false;

            if (!zen::serializers::deserialize_raw(value_max, in))
                return false;

            set_value(value);
            set_value_min(value_min);
            set_value_max(value_max);

            return in.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const
        {
            bool attributes_changed =   (m_value_min != reference.m_value_min) ||
                                        (m_value_max != reference.m_value_max);

            bool value_changed = (m_value != reference.m_value);

            if (!attributes_changed && !value_changed)
                return false;

            zen::serializers::serialize_boolean(value_changed, out);
            zen::serializers::serialize_boolean(attributes_changed, out);

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

                if (num_bits_changed)
                {
                    zen::serializers::serialize_raw(m_num_bits, out);
                }
            }

            if (value_changed)
            {
                zen::serializers::serialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, out);
            }

            return out.ok();
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in)
        {
            bool attributes_changed;
            bool value_changed;
            zen::serializers::deserialize_boolean(value_changed, in);
            zen::serializers::deserialize_boolean(attributes_changed, in);

            if (attributes_changed)
            {
                bool value_min_changed;
                bool value_max_changed;
                zen::serializers::deserialize_boolean(value_min_changed, in);
                zen::serializers::deserialize_boolean(value_max_changed, in);
                
                if (value_min_changed)
                {
                    zen::serializers::deserialize_raw(m_value_min, in);
                }

                if (value_max_changed)
                {
                    zen::serializers::deserialize_raw(m_value_max, in);
                }
            }

            if (value_changed)
            {
                zen::serializers::deserialize_float_ranged(m_value, m_value_min, m_value_max, m_num_bits, in);
            }

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

            return true;
        }

        template<typename TYPE>
        TYPE IntegerRanged<TYPE>::get_value_max() const
        {
            return m_value_max;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator == (const IntegerRanged& rhs) const
        {
            if (m_value != rhs.m_value)
                return false;

            if (m_value_min != rhs.m_value_min)
                return false;

            if (m_value_max != rhs.m_value_max)
                return false;

            return true;
        }

        template<typename TYPE>
        bool IntegerRanged<TYPE>::operator != (const IntegerRanged& rhs) const
        {
            return !((*this) == rhs);
        }

        template<typename TYPE>
        IntegerRanged<TYPE>& IntegerRanged<TYPE>::operator = (const IntegerRanged<TYPE>& rhs)
        {
            set_value(rhs.m_value);
            set_value_min(rhs.m_value_min);
            set_value_max(rhs.m_value_max);
            return *this;
        }
    }
}
