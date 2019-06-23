#include "zen/data/zen_data_component.h"
#include "zen/debug/zen_debug_assert.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        Component::Component(Factory* factory, Factory::RegistryId registry_id, Element& element)
            : m_factory(factory)
            , m_registry_id(registry_id)
            , m_element(&element)
            , m_is_reference(true)
        {}

        Component::Component(Factory* factory)
            : m_factory(factory)
            , m_registry_id(Factory::INVALID_REGISTRY_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Component::Component()
            : m_factory(nullptr)
            , m_registry_id(Factory::INVALID_REGISTRY_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Component::~Component()
        {
            if (!m_is_reference)
            {
                delete m_element;
            }
        }

        bool Component::serialize_full(bitstream::Writer& out) const
        {
            zen::serializers::serialize_raw(m_registry_id, out);
            m_element->serialize_full(out);

            return out.ok();
        }

        bool Component::deserialize_full(bitstream::Reader& in)
        {
            Factory::RegistryId element_registry_id;
            if (!zen::serializers::deserialize_raw(element_registry_id, in))
                return false;
            
            if (element_registry_id != m_registry_id)
            {
                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_registry_id = element_registry_id;

                m_element = m_factory->construct_element(element_registry_id);

                set_touched(true);
            }

            return m_element->deserialize_full(in);
        }

        bool Component::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Component& reference = (const Component&) element_reference;

            bool type_id_changed = (m_registry_id != reference.m_registry_id);

            if (!serializers::serialize_boolean(type_id_changed, delta_bits))
                return false;

            if (!type_id_changed)
            {
                return m_element->serialize_delta(reference, out, delta_bits);
            }
            else
            {
                if (!m_factory->serialize_registry_id(m_registry_id, out))
                    return false;
            
                // create default element to delta against.
                Element* base = m_factory->construct_element(m_registry_id);

                m_element->serialize_delta(*base, out, delta_bits);
                
                // free base element.
                delete base;

                return out.ok();
            }
        }

        bool Component::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Component& reference = (const Component&) element_reference;

            bool type_id_changed;
            if (!serializers::deserialize_boolean(type_id_changed, delta_bits))
                return false;

            if (!type_id_changed)
            {
                return m_element->deserialize_delta(reference, in, delta_bits);
            }
            else
            {
                Factory::RegistryId element_registry_id;
                if (!m_factory->deserialize_registry_id(element_registry_id, in))
                    return false;

                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_registry_id = element_registry_id;
                
                m_element = m_factory->construct_element(element_registry_id);

                Element* base = m_factory->construct_element(element_registry_id);

                m_element->deserialize_delta(*base, in, delta_bits);
                
                delete base;
                return in.ok();
            }
        }

        bool Component::operator == (const Element& element_rhs) const
        {
            const Component& rhs = (const Component&)element_rhs;

            if (m_registry_id != rhs.m_registry_id)
                return false;

            return (*m_element == *rhs.m_element);
        }

        bool Component::operator != (const Element& element_rhs) const
        {
            const Component& rhs = (const Component&) element_rhs;

            return !((*this) == rhs);
        }

        Element& Component::operator = (const Element& element_rhs)
        {
            const Component& rhs = (const Component&) element_rhs;

            return (*this).operator=(rhs);
        }

        Component& Component::operator = (const Component& rhs)
        {
            if (m_registry_id != rhs.m_registry_id)
            {
                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_registry_id = rhs.m_registry_id;

                m_element = m_factory->construct_element(m_registry_id);
            }

            *m_element = *rhs.m_element;

            return *this;
        }

        void Component::debug_randomize(debug::Randomizer& randomizer)
        {
            // cleanup.
            if (!m_is_reference)
                delete m_element;

            m_registry_id = Factory::INVALID_REGISTRY_ID;
            m_element = nullptr;

            // randomize type.
            m_registry_id = randomizer.get_integer_ranged((size_t)0, m_factory->get_registry_size() - 1);
            
            // randomize object.
            m_element = m_factory->construct_element(m_registry_id);
            m_element->debug_randomize(randomizer);
        }
    }
}
