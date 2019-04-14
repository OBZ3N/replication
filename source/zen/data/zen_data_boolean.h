#pragma once

namespace zen
{
    namespace data
    {
        class Boolean
        {
        public:
            Boolean(bool value = false, bool value_default = false);

        protected:
            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const Boolean& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Boolean& reference, bitstream::Reader& in);

            bool set_value(bool value);
            bool get_value() const;

            bool operator == (const Boolean& rhs) const;
            bool operator != (const Boolean& rhs) const;

        private:
            bool m_value;
            bool m_value_default;
        };
    }
}

#include "zen/data/zen_data_boolean.hpp"
