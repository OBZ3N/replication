#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/data/zen_data_factory.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class Mutable : public Element
        {
        public:
            Mutable(Factory* factory, uint32_t type_id, Element& element);
            Mutable(Factory* factory);
            Mutable();
            ~Mutable();

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const Mutable& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Mutable& reference, bitstream::Reader& in);

            inline bool operator == (const Mutable& rhs) const;
            inline bool operator != (const Mutable& rhs) const;

            Mutable& operator = (const Mutable& rhs);

            bool is_null() const { return m_element == nullptr; }

            Factory* m_factory;
            uint32_t m_element_type_id;
            Element* m_element;
            bool     m_is_reference;
        };
    }
}
