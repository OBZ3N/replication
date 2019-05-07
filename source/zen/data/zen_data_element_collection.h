#pragma once

#include "zen/data/zen_data_vector.h"
#include "zen/data/zen_data_element.h"
#include "zen/data/zen_data_factory.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class TypedElement : public Element
        {
        public:
            TypedElement(Factory* factory, uint32_t type_id, Element& element);
            TypedElement(Factory* factory);
            TypedElement();
            ~TypedElement();

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const TypedElement& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const TypedElement& reference, bitstream::Reader& in);

            inline bool operator == (const Vector& rhs) const;
            inline bool operator != (const Vector& rhs) const;

            TypedElement& operator = (const TypedElement& rhs);

            bool is_null() const { return m_element == nullptr; }

            Factory* m_factory;
            uint32_t m_element_type_id;
            Element* m_element;
            bool     m_is_reference;
        };

        template<typename TYPE>
        class ElementCollection : public Vector<TypedElement>
        {
        public:
            ElementCollection(Factory* factory = nullptr, size_t capcity = 0);

            ElementCollection& operator = (const ElementCollection& rhs);

        private:
            Factory* m_factory;
        };
    }
}
