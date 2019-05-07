#include "zen/data/zen_data_mutable.h"
#include "zen/debug/zen_debug_assert.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        Mutable::Mutable(Factory* factory, uint32_t type_id, Element& element)
            : m_factory(factory)
            , m_element_type_id(type_id)
            , m_element(&element)
            , m_is_reference(true)
        {}

        Mutable::Mutable(Factory* factory)
            : m_factory(factory)
            , m_element_type_id((uint32_t)-1)
            , m_element(nullptr)
            , m_is_reference(true)
        {}

        Mutable::Mutable()
            : m_factory(nullptr)
            , m_element_type_id((uint32_t)-1)
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
            if (!zen::serializers::serialize_raw(m_element_type_id, out))
                return false;

            if (!m_element->serialize_full(out))
                return false;

            return true;
        }

        bool Mutable::deserialize_full(bitstream::Reader& in)
        {
            uint32_t element_type_id;
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

        bool Mutable::serialize_delta(const Mutable& reference, bitstream::Writer& out) const
        {
            bool different_type_id = (m_element_type_id != reference.m_element_type_id);

            size_t write_pos = out.bitcount();

            if (!serializers::serialize_boolean(different_type_id, out))
                return false;

            if (!different_type_id)
            {
                if (!m_element->serialize_delta(reference, out))
                {
                    // roll back 
                    out.set_bitcount(write_pos);
                    return false;
                }

                return out.ok();
            }
            else
            {
                if (!zen::serializers::serialize_integer_ranged(m_element_type_id, 0, m_factory->get_num_types()))
                {
                    // roll back buffer pointer.
                    out.set_bitcount(write_pos);
                    return false;
                }

                // create default element to delta against.
                Element* base = m_factory->construct_element(m_element_type_id);

                if (!m_element->serialize_delta(*base, out))
                {
                    // free base element.
                    delete base;

                    out.set_bitcount(write_pos);
                    return false;
                }
                else
                {
                    // free base element.
                    delete base;
                    
                    return out.ok();
                }
            }
        }

        bool Mutable::deserialize_delta(const Mutable& reference, bitstream::Reader& in)
        {
            bool different_type_id;
            if (!serializers::deserialize_boolean(different_type_id, in))
                return false;

            if (!different_type_id)
            {
                if (!m_element->deserialize_delta(reference, in))
                    return false;
                
                return in.ok();
            }
            else
            {
                uint32_t element_type_id;
                if (!serializers::deserialize_integer_ranged(element_type_id, 0, m_factory->get_num_types()))
                    return false;

                if (!m_is_reference)
                {
                    delete m_element;
                }

                m_is_reference = false;

                m_element_type_id = element_type_id;
                
                m_element = m_factory->construct_element(element_type_id);

                Element* base = m_factory->construct_element(element_type_id);

                if (!m_element->deserialize_delta(*base, in))
                {
                    delete base;

                    return false;
                }

                delete base;

                return in.ok();
            }
        }


        bool Mutable::operator == (const Mutable& rhs) const
        {
            if (m_element_type_id != rhs.m_element_type_id)
                return false;

            return (*m_element == *rhs.m_element);
        }

        bool Mutable::operator != (const Mutable& rhs) const
        {
            return !((*this) == rhs);
        }

        Mutable& Mutable::operator = (const Mutable& rhs)
        {
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
