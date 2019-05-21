#pragma once

#include "zen/data/zen_data_element.h"

namespace zen
{
    namespace data
    {
        class Boolean : public Element
        {
        public:
            Boolean();
            Boolean(bool value);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            bool set_value(bool value);
            bool get_value() const;

            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            inline Element& operator = (const Element& rhs) override;

            void debug_randomize(debug::Randomizer& randomizer) override;

        private:
            bool m_value;
        };
    }
}

#include "zen/data/zen_data_boolean.hpp"
