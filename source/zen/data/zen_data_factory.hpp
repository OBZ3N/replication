
#pragma once

#include "zen/data/zen_data_factory.h"

namespace zen
{
    namespace data
    {
        template<typename Type>
        uint32_t Factory::register_element_type(const char* type_name)
        {
            Item item;

            item.m_type_name = class_name;

            item.m_type_id = (uint32_t)m_type_registry.size();

            item.m_constructor_func = element_constructor<Type>;

            m_registry.push_back(item);

            m_num_type_ids = (uint32_t)m_registry.size();

            m_num_bits = zen::serializers::num_bits_required(m_num_type_ids - 1);

            m_table[class_name] = m_registry.back();

            return item.m_factory_id;
        }

        template<typename Type>
        uint32_t Factory::get_element_type_id() const
        {
            return TypeRegistrar<Type>::s_factory_type_id;
        }

        template<typename DerivedType>
        DerivedType* Factory::element_constructor()
        {
            return new Type;
        }
    }
}
