#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/data/zen_data_factory.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class Component : public Element
        {
        public:
            DECLARE_RTTI_TYPE_ID();

            Component(Element* container=nullptr);
            Component(Factory* factory, Element* container = nullptr);
            Component(Factory* factory, Factory::RegistryId registry_id, Element& element, Element* container = nullptr);
            ~Component();

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;
            Component& operator = (const Component& rhs);

            bool is_null() const { return m_element == nullptr; }

            void debug_randomize(debug::Randomizer& randomizer, float probability) override;

        private:
            void set_registry_id(Factory::RegistryId registry_id);

            Factory*            m_factory;
            Factory::RegistryId m_registry_id;
            Element*            m_element;
            bool                m_is_reference;
        };
    }
}
