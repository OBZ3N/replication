#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class String : public Element
        {
        public:
            DECLARE_RTTI_TYPE_ID();

            String();
            String(const std::string& value);
            String(const char* value);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            bool set_value(const std::string& value);
            const std::string& get_value() const;

            bool operator == (const Element& rhs) const override;
            bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;
            String& operator = (const String& rhs);

            void debug_randomize_full(debug::Randomizer& randomizer) override;
            void debug_randomize_delta(const Element& reference, debug::Randomizer& randomizer) override;

        private:
            std::string m_value;
        };
    }
}

#include "zen/data/zen_data_string.hpp"
