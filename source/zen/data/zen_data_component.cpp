#include "zen/data/zen_data_component.h"
#include "zen/debug/zen_debug_assert.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        Component::Component(Factory* factory, Factory::RegistryId registry_id, Element& element, Element* container)
            : Element(container)
            , m_factory(factory)
            , m_registry_id(registry_id)
            , m_element(&element)
            , m_is_reference(true)
        {}

        Component::Component(Factory* factory, Element* container)
            : Element(container)
            , m_factory(factory)
            , m_registry_id(Factory::INVALID_REGISTRY_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Component::Component(Element* container)
            : Element(container)
            , m_factory(nullptr)
            , m_registry_id(Factory::INVALID_REGISTRY_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        void Component::set_registry_id(Factory::RegistryId registry_id)
        {
            if (registry_id == m_registry_id)
                return;
        
            // cleanup.
            if (!m_is_reference)
            {
                m_factory->destruct_element(m_registry_id, m_element);
            }

            m_registry_id  = Factory::INVALID_REGISTRY_ID;
            m_element      = nullptr;
            m_is_reference = false;

            if (registry_id == Factory::INVALID_REGISTRY_ID)
                return;

            // instantiate object.
            m_element = m_factory->construct_element(registry_id, this);

            m_registry_id = registry_id;

            set_touched(true);
        }

        Component::~Component()
        {
            set_registry_id(Factory::INVALID_REGISTRY_ID);
       }

        bool Component::serialize_full(bitstream::Writer& out) const
        {
            if (!zen::serializers::serialize_raw(m_registry_id, out))
                return false;

            return m_element->serialize_full(out);
        }

        bool Component::deserialize_full(bitstream::Reader& in)
        {
            Factory::RegistryId element_registry_id;
            if (!zen::serializers::deserialize_raw(element_registry_id, in))
                return false;

            // update element registry id.
            set_registry_id(element_registry_id);

            if (!m_element->deserialize_full(in))
                return false;

            return in.ok();
        }

        bool Component::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Component& reference = (const Component&) element_reference;

            bool registry_id_changed = (m_registry_id != reference.m_registry_id);

            if (!serializers::serialize_boolean(registry_id_changed, delta_bits))
                return false;

            if (!registry_id_changed)
            {
                return m_element->serialize_delta(reference, out, delta_bits);
            }
            else
            {
                if (!m_factory->serialize_registry_id(m_registry_id, out))
                    return false;
            
                // create default element to delta against.
                const Element* base = m_factory->get_base_element(m_registry_id);

                return m_element->serialize_delta(*base, out, delta_bits);
            }
        }

        bool Component::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Component& reference = (const Component&) element_reference;

            bool registry_id_changed;
            if (!serializers::deserialize_boolean(registry_id_changed, delta_bits))
                return false;

            Factory::RegistryId registry_id = reference.m_registry_id;

            if (registry_id_changed)
            {
                if (!m_factory->deserialize_registry_id(registry_id, in))
                    return false;
            }

            set_registry_id(registry_id);

            const Element* base = m_factory->get_base_element(m_registry_id);

            return m_element->deserialize_delta(*base, in, delta_bits);
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
            set_registry_id(rhs.m_registry_id);

            *m_element = *rhs.m_element;

            return *this;
        }

        void Component::debug_randomize(debug::Randomizer& randomizer, float probability)
        {
            if (randomizer.get_float_ranged(1.0f) < probability)
            {
                // randomize type.
                Factory::RegistryId registry_id = randomizer.get_integer_ranged((size_t)0, m_factory->get_registry_size() - 1);

                set_registry_id(registry_id);
            }

            if (m_element != nullptr)
            {
                m_element->debug_randomize(randomizer, probability);
            }
        }

        bool Component::sanity_check() const
        {
            return true;
        }
    }
}
