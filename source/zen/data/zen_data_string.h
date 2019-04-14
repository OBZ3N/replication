#pragma once

namespace zen
{
    namespace data
    {
        class String
        {
        public:
            String(const std::string& value="", const std::string& value_default="");
            String(const char* value="", const char* value_default = "");

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const String& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const String& reference, bitstream::Reader& in);

            bool set_value(const std::string& value);
            const std::string& get_value() const;

            bool operator == (const String& rhs) const;
            bool operator != (const String& rhs) const;

        private:
            std::string m_value;
            std::string m_value_default;
        };
    }
}

#include "zen/data/zen_data_string.hpp"
