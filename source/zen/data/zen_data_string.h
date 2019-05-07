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
            String();
            String(const std::string& value);
            String(const char* value);

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const String& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const String& reference, bitstream::Reader& in);

            bool set_value(const std::string& value);
            const std::string& get_value() const;

            bool operator == (const String& rhs) const;
            bool operator != (const String& rhs) const;

            String& operator = (const String& rhs);

        private:
            std::string m_value;
        };
    }
}

#include "zen/data/zen_data_string.hpp"
