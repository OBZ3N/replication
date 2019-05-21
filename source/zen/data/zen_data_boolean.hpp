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

        inline bool Boolean::operator == (const Element& element_rhs) const
        {
            const Boolean& rhs = (const Boolean&)element_rhs;

            if (m_value != rhs.m_value)
                return false;

            return true;
        }

        inline bool Boolean::operator != (const Element& element_rhs) const
        {
            const Boolean& rhs = (const Boolean&)element_rhs;

            return !((*this) == rhs);
        }

        inline Element& Boolean::operator = (const Element& element_rhs)
        {
            const Boolean& rhs = (const Boolean&)element_rhs;

            set_value(rhs.m_value);

            return *this;
        }

        inline bool Boolean::serialize_full(bitstream::Writer& out) const
        {
            return zen::serializers::serialize_boolean(m_value, out);
        }

        inline bool Boolean::deserialize_full(bitstream::Reader& in)
        {
            bool value;
            zen::serializers::deserialize_boolean(value, in);
            if (!in.ok()) return false;

            set_value(value);

            return true;
        }

        inline bool Boolean::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Boolean& element = (const Boolean&)element_reference;

            bool value_changed = ((*this) != reference);
            serializers::serialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                zen::serializers::serialize_boolean(m_value, out);
            }

            return out.ok();
        }

        inline bool Boolean::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Boolean& element = (const Boolean&)element_reference;

            bool value_changed;
            zen::serializers::deserialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                bool value;
                zen::serializers::deserialize_boolean(value, in);

                if (!in.ok())
                    return false;

                set_value(value);
            }

            return in.ok();
        }

        void Boolean::debug_randomize(zen::debug::Randomizer& randomizer)
        {
            uint32_t i = randomizer.get_integer_ranged(i, 1);
            set_value((i == 1));

        }
    }
}
