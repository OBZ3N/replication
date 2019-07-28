#pragma once

#include "zen/data/zen_data_element.h"

namespace zen
{
    namespace data
    {
        class Boolean : public Element
        {
        public:
            DECLARE_RTTI_TYPE_ID();

            Boolean(Element* container=nullptr);
            Boolean(bool value, Element* container = nullptr);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            bool set_value(bool value);
            bool get_value() const;

            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            inline Element& operator = (const Element& rhs) override;
            Boolean& operator = (const Boolean& rhs);

            void debug_randomize_full(debug::Randomizer& randomizer) override;
            void debug_randomize_delta(const Element& reference, debug::Randomizer& randomizer) override;

        private:
            bool m_value;
        };
    }
}

#include "zen/data/zen_data_boolean.hpp"
