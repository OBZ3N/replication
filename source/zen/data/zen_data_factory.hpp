
#pragma once

#include "zen/data/zen_data_factory.h"

namespace zen
{
    namespace data
    {

        template<typename Class> bool Factory::is_class_registered() const
        {
            auto it = m_type_lookup.find(Class::get_type_id_instance().get_instance_id());

            return it != m_type_lookup.end();
        }

        template<typename Class> Factory::RegistryId Factory::register_class()
        {
            return register_internal(Class::get_type_id_instance(), construct_type<Class>, destruct_type<Class>);
        }

        template<typename Type> 
        Type* Factory::construct_type()
        {
            return new Type;
        }

        template<typename Type> 
        void Factory::destruct_type(Element*& element)
        {
            if (element)
                delete element;

            element = nullptr;
        }
    }
}
