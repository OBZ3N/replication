#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class Boolean : public Element
        {
        public:
            Boolean();
            Boolean(bool value);

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);

            bool serialize_delta(const Boolean& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Boolean& reference, bitstream::Reader& in);

            bool set_value(bool value);
            bool get_value() const;

            inline bool operator == (const Boolean& rhs) const;
            inline bool operator != (const Boolean& rhs) const;

            inline Boolean& operator = (const Boolean& rhs);

        private:
            bool m_value;
        };
    }
}

#include "zen/data/zen_data_boolean.hpp"
