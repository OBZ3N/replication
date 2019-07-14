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
        class Factory
        {
        public:
            typedef int32_t RegistryId;

            static constexpr RegistryId INVALID_REGISTRY_ID = (RegistryId)(-1);

            Factory();
            ~Factory();

            void register_zen_data_types();

            template<typename Class> bool is_class_registered() const;
            template<typename Class> RegistryId register_class();

            const zen::data::Element* get_base_element(RegistryId registry_id) const;
            const zen::rtti::TypeId* get_type_id(RegistryId registry_id) const;
            RegistryId get_registry_id(const zen::rtti::TypeId& type_id) const;
            
            Element* construct_element(RegistryId type_id) const;
            void destruct_element(RegistryId type_id, Element*& element) const;

            size_t get_registry_size() const;

            bool serialize_registry_id(RegistryId type_id, zen::bitstream::Writer& out) const;
            bool deserialize_registry_id(RegistryId& type_id, zen::bitstream::Reader& in) const;

        private:
            template<typename Type> static Type* construct_type();
            template<typename Type> static void destruct_type(Element*& element);
            RegistryId register_internal(const zen::rtti::TypeId& type_id, std::function<Element*()> constructor, std::function<void(Element*&)> destructor);

            struct RegistryItem
            {
                const zen::rtti::TypeId*        m_type_id;
                RegistryId                      m_registry_id;
                Element*                        m_base_element;
                std::function<Element*()>       m_constructor_func;
                std::function<void(Element*&)>  m_destructor_func;
            };

            std::unordered_map<int32_t, RegistryId> m_type_lookup;
            std::vector<RegistryItem>               m_type_registry;

            RegistryId m_registry_id_last;
            size_t m_registry_id_num_bits;
        };
    }
}

#include "zen/data/zen_data_factory.hpp"
