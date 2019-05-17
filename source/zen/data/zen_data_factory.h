#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        typedef int32_t TypeId;

        class Factory
        {
        public:
            static constexpr TypeId INVALID_TYPE_ID = (TypeId)(-1);

            Factory();

            template<typename Type>
            TypeId register_element_type(const char* type_name);

            template<typename Type>
            TypeId get_element_type_id() const;

            const char* get_type_name(TypeId type_id) const;
            TypeId get_type_id(const char* type_name) const;

            size_t get_num_types() const;
            Element* construct_element(TypeId type_id) const;

            bool serialize_type_id(TypeId type_id, zen::bitstream::Writer& out);
            bool deserialize_type_id(TypeId& type_id, zen::bitstream::Reader& in);

        private:
            template<typename DerivedType> 
            DerivedType* element_constructor();

            struct Item
            {
                const char*                 m_type_name;
                TypeId                      m_type_id;
                std::function<Element*()>   m_constructor_func;
            };

            std::unordered_map<std::string, Item>   m_type_table;
            std::vector<Item>                       m_type_registry;

            TypeId m_type_id_max;
            size_t m_type_id_num_bits;
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
            static TypeId s_type_id;
        };
    }
}

#define ZEN_REGISTER_ELEMENT_TYPE(Type, factory) { static TypeRegistrar<Type> s_##Type_instance; s_##Type_instance::s_type_id = factory.register_element_type<Type>(#Type); }

#define ZEN_REGISTER_CUSTOM_ELEMENT_TYPE(Type, factory) factory.register_element_type<Type>(#Type)

#include "zen/data/zen_data_factory.hpp"
