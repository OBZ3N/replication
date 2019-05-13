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

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;

            bool is_null() const { return m_element == nullptr; }

            Factory* m_factory;
            uint32_t m_element_type_id;
            Element* m_element;
            bool     m_is_reference;
        };
    }
}
