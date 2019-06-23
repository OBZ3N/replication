#pragma once

#include "zen/data/zen_data_string.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_string.h"

namespace zen
{
    namespace data
    {
        inline String::String()
        {}

        inline String::String(const std::string& value)
            : m_value(value)
        {}

        inline String::String(const char* value)
            : m_value(value)
        {}

        inline Element& String::operator = (const Element& element_rhs)
        {
            const String& rhs = (const String&)element_rhs;

            set_value(rhs.m_value);

            return *this;
        }
        
        inline bool String::serialize_full(bitstream::Writer& out) const
        {
            return zen::serializers::serialize_string(m_value, out);
        }

        inline bool String::deserialize_full(bitstream::Reader& in)
        {
            std::string value;
            zen::serializers::deserialize_string(value, in);

            if(in.ok())
                set_value(value);

            return in.ok();
        }

        inline bool String::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const String& reference = (const String&)element_reference;

            bool value_changed = (m_value != reference.m_value);
            serializers::serialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                zen::serializers::serialize_string(m_value, out);
            }

            return out.ok();
        }

        inline bool String::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const String& reference = (const String&) element_reference;
            
            bool value_changed;
            serializers::deserialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                std::string value;
                zen::serializers::deserialize_string(value, in);
                if (in.ok())
                    set_value(value);
            }

            return in.ok();
        }

        inline bool String::set_value(const std::string& value)
        {
            if (m_value == value)
                return false;

            m_value = value;

            set_touched(true);

            return true;
        }

        inline const std::string& String::get_value() const
        {
            return m_value;
        }

        inline bool String::operator == (const Element& element_rhs) const
        {
            const String& rhs = (const String&)element_rhs;

            return m_value == rhs.m_value;
        }

        inline bool String::operator != (const Element& element_rhs) const
        {
            const String& rhs = (const String&)element_rhs;

            return m_value != rhs.m_value;
        }

        inline void String::debug_randomize(debug::Randomizer& randomizer)
        {
            std::string value;

            size_t num_characters = randomizer.get_integer_ranged<size_t>(8, 100);

            for (size_t i = 0; i < num_characters; ++i)
            {
                uint8_t character = (uint8_t)randomizer.get_integer_ranged<uint32_t>('0', 'z');

                value.push_back(character);
            }
        }
    }
}