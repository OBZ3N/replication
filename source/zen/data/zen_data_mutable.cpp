#include "zen/data/zen_data_mutable.h"
#include "zen/debug/zen_debug_assert.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        Mutable::Mutable(Factory* factory, Factory::TypeId type_id, Element& element)
            : m_factory(factory)
            , m_element_type_id(type_id)
            , m_element(&element)
            , m_is_reference(true)
        {}

        Mutable::Mutable(Factory* factory)
            : m_factory(factory)
            , m_element_type_id(Factory::INVALID_TYPE_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Mutable::Mutable()
            : m_factory(nullptr)
            , m_element_type_id(Factory::INVALID_TYPE_ID)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Mutable::~Mutable()
        {
            if (!m_is_reference)
            {
                delete m_element;
            }
        }

        bool Mutable::serialize_full(bitstream::Writer& out) const
        {
            zen::serializers::serialize_raw(m_element_type_id, out);
            m_element->serialize_full(out);

            return out.ok();
        }

        bool Mutable::deserialize_full(bitstream::Reader& in)
        {
            Factory::TypeId element_type_id;
            if (!zen::serializers::deserialize_raw(element_type_id, in))
                return false;
            
            if (element_type_id != m_element_type_id)
            {
                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_element_type_id = element_type_id;

                m_element = m_factory->construct_element(element_type_id);

                set_touched(true);
            }

            return m_element->deserialize_full(in);
        }

        bool Mutable::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Mutable& reference = (const Mutable&) element_reference;

            bool type_id_changed = (m_element_type_id != reference.m_element_type_id);

            if (!serializers::serialize_boolean(type_id_changed, delta_bits))
                return false;

            if (!type_id_changed)
            {
                return m_element->serialize_delta(reference, out, delta_bits);
            }
            else
            {
                if (!m_factory->serialize_type_id(m_element_type_id, out))
                    return false;
            
                // create default element to delta against.
                Element* base = m_factory->construct_element(m_element_type_id);

                m_element->serialize_delta(*base, out, delta_bits);
                
                // free base element.
                delete base;

                return out.ok();
            }
        }

        bool Mutable::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Mutable& reference = (const Mutable&) element_reference;

            bool type_id_changed;
            if (!serializers::deserialize_boolean(type_id_changed, delta_bits))
                return false;

            if (!type_id_changed)
            {
                return m_element->deserialize_delta(reference, in, delta_bits);
            }
            else
            {
                Factory::TypeId element_type_id;
                if (!m_factory->deserialize_type_id(element_type_id, in))
                    return false;

                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_element_type_id = element_type_id;
                
                m_element = m_factory->construct_element(element_type_id);

                Element* base = m_factory->construct_element(element_type_id);

                m_element->deserialize_delta(*base, in, delta_bits);
                
                delete base;
                return in.ok();
            }
        }

        bool Mutable::operator == (const Element& element_rhs) const
        {
            const Mutable& rhs = (const Mutable&)element_rhs;

            if (m_element_type_id != rhs.m_element_type_id)
                return false;

            return (*m_element == *rhs.m_element);
        }

        bool Mutable::operator != (const Element& element_rhs) const
        {
            const Mutable& rhs = (const Mutable&) element_rhs;

            return !((*this) == rhs);
        }

        Element& Mutable::operator = (const Element& element_rhs)
        {
            const Mutable& rhs = (const Mutable&) element_rhs;

            if (m_element_type_id != rhs.m_element_type_id)
            {
                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_element_type_id = rhs.m_element_type_id;

                m_element = m_factory->construct_element(m_element_type_id);
            }

            *m_element = *rhs.m_element;

            return *this;
        }
    }
}
