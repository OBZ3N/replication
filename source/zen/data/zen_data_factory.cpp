
#include "zen/data/zen_data_factory.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        Factory::Factory()
            : m_num_type_ids(0)
            , m_num_bits(0)
        {}

        const char* Factory::get_type_name(uint32_t type_id) const
        {
            return m_type_registry[type_id].m_type_name;
        }

        uint32_t Factory::get_type_id(const char* type_name) const
        {
            auto it = m_type_table.find(type_name);

            if (it == m_type_table.end())
            {
                return (uint32_t)-1;
            }

            return it->second.m_type_id;
        }

        uint32_t Factory::get_num_types() const
        {
            return (uint32_t)m_type_registry.size();
        }

        Element* Factory::construct_element(uint32_t type_id) const
        {
            return m_type_registry[type_id].m_constructor_func();
        }

        bool Factory::serialize_type_id(uint32_t type_id, zen::bitstream::Writer& out)
        {
            return zen::serializers::serialize_integer_ranged(type_id, 0u, m_num_type_ids, m_num_bits, out);
        }

        bool Factory::deserialize_type_id(uint32_t& type_id, zen::bitstream::Reader& in)
        {
            return zen::serializers::deserialize_integer_ranged(type_id, 0u, m_num_type_ids, m_num_bits, in);
        }
    }
}
