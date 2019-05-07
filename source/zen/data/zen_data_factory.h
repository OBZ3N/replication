#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "zen/data/zen_data_element.h"

namespace zen
{
    namespace data
    {
        class Factory
        {
        public:
            Factory();

            template<typename Type>
            uint32_t register_element_type(const char* type_name);

            template<typename Type>
            uint32_t  get_element_type_id() const;

            const char* get_type_name(uint32_t type_id) const;
            uint32_t    get_type_id(const char* type_name) const;

            uint32_t  get_num_types() const;
            Element* construct_element(uint32_t type_id) const;

        private:
            template<typename DerivedType> 
            DerivedType* element_constructor();

            struct Item
            {
                const char*                 m_type_name;
                uint32_t                    m_type_id;
                std::function<Element*()>   m_constructor_func;
            };

            std::unordered_map<std::string, Item>   m_type_table;
            std::vector<Item>                       m_type_registry;
        };
    }
}

namespace zen
{
    namespace data
    {
        template<typename Type>
        struct TypeRegistrar
        {
            static uint32_t s_type_id;
        };
    }
}

#define ZEN_REGISTER_ELEMENT_TYPE(Type, factory) { static TypeRegistrar<Type> s_##Type_instance; s_##Type_instance::s_type_id = factory.register_element_type<Type>(#Type); }

#define ZEN_REGISTER_CUSTOM_ELEMENT_TYPE(Type, factory) factory.register_element_type<Type>(#Type)

#include "zen/data/zen_data_factory.hpp"
