#pragma once

#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class Element
        {
        public:
            Element();
            virtual ~Element();

            void set_touched(bool value);
            bool is_touched() const;

            virtual bool serialize_full(bitstream::Writer& out) const = 0;
            virtual bool deserialize_full(bitstream::Reader& in) = 0;

            virtual bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const = 0;
            virtual bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) = 0;

            virtual bool operator == (const Element& rhs) const = 0;
            virtual bool operator != (const Element& rhs) const = 0;

            virtual Element& operator = (const Element& rhs) = 0;

        private:
            bool m_touched;
        };
    }
}

#include "zen/data/zen_data_element.hpp"
