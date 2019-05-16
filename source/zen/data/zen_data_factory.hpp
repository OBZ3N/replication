
#pragma once

#include "zen/data/zen_data_factory.h"

namespace zen
{
    namespace data
    {
        template<typename Type>
        TypeId Factory::register_element_type(const char* type_name)
        {
            Item item;

            item.m_type_name = type_name;

            item.m_type_id = (TypeId)m_type_registry.size();

            item.m_constructor_func = element_constructor<Type>;

            m_type_registry.push_back(item);

            m_type_table[type_name] = item;

            m_type_id_max = item.m_type_id;

            m_num_bits = zen::serializers::num_bits_required(INVALID_TYPE_ID, m_type_id_max);

            return item.m_type_id;
        }

        template<typename Type>
        TypeId Factory::get_element_type_id() const
        {
            return TypeRegistrar<Type>::s_type_id;
        }

        template<typename DerivedType>
        DerivedType* Factory::element_constructor()
        {
            return new Type;
        }
    }
}
