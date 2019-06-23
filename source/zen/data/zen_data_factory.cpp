
#include "zen/data/zen_data_factory.h"
#include "zen/data/zen_data_boolean.h"
#include "zen/data/zen_data_component.h"
#include "zen/data/zen_data_integer_ranged.h"
#include "zen/data/zen_data_float_ranged.h"
#include "zen/data/zen_data_raw.h"
#include "zen/data/zen_data_vector.h"
#include "zen/data/zen_data_string.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"
#include "zen/rtti/zen_rtti_base.h"

namespace zen
{
    namespace data
    {
        Factory::Factory()
            : m_registry_id_last(0)
            , m_registry_id_num_bits(0)
        {}

        const zen::rtti::TypeId* Factory::get_type_id(Factory::RegistryId registry_id) const
        {
            return m_type_registry[registry_id].m_type_id;
        }

        Factory::RegistryId Factory::get_registry_id(const zen::rtti::TypeId& type_id) const
        {
            auto it = m_type_lookup.find(type_id.get_instance_id());

            return (it == m_type_lookup.end()) ? Factory::INVALID_REGISTRY_ID : it->second;
        }

        Factory::RegistryId Factory::register_internal(const zen::rtti::TypeId& type_id, std::function<Element*()> constructor, std::function<void(Element*&)> destructor)
        {
            RegistryItem item;
            item.m_type_id          = &type_id;
            item.m_registry_id      = (Factory::RegistryId)m_type_registry.size();
            item.m_constructor_func = constructor;
            item.m_destructor_func  = destructor;

            m_type_registry.push_back(item);
            m_type_lookup[type_id.get_instance_id()] = item.m_registry_id;

            m_registry_id_last = item.m_registry_id;
            m_registry_id_num_bits = zen::serializers::number_of_bits_required(INVALID_REGISTRY_ID, m_registry_id_last);

            return item.m_registry_id;
        }

        void Factory::register_zen_data_types()
        {
            register_class<zen::data::Component>();
            register_class<zen::data::Boolean>();
            register_class<zen::data::String>();
            register_class<zen::data::Raw<bool>>();
            register_class<zen::data::Raw<uint8_t>>();
            register_class<zen::data::Raw<uint16_t>>();
            register_class<zen::data::Raw<uint32_t>>();
            register_class<zen::data::Raw<uint64_t>>();
            register_class<zen::data::Raw<int8_t>>();
            register_class<zen::data::Raw<int16_t>>();
            register_class<zen::data::Raw<int32_t>>();
            register_class<zen::data::Raw<int64_t>>();
            register_class<zen::data::Raw<float>>();
            register_class<zen::data::Raw<double>>();
            register_class<zen::data::IntegerRanged<uint8_t>>();
            register_class<zen::data::IntegerRanged<uint16_t>>();
            register_class<zen::data::IntegerRanged<uint32_t>>();
            register_class<zen::data::IntegerRanged<uint64_t>>();
            register_class<zen::data::IntegerRanged<int8_t>>();
            register_class<zen::data::IntegerRanged<int16_t>>();
            register_class<zen::data::IntegerRanged<int32_t>>();
            register_class<zen::data::IntegerRanged<int64_t>>();
            register_class<zen::data::FloatRanged<float>>();
            register_class<zen::data::FloatRanged<double>>();

            register_class<zen::data::Vector<zen::data::Component>>();
            register_class<zen::data::Vector<zen::data::Boolean>>();
            register_class<zen::data::Vector<zen::data::String>>();
            register_class<zen::data::Vector<zen::data::Raw<bool>>>();
            register_class<zen::data::Vector<zen::data::Raw<uint8_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<uint16_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<uint32_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<uint64_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<int8_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<int16_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<int32_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<int64_t>>>();
            register_class<zen::data::Vector<zen::data::Raw<float>>>();
            register_class<zen::data::Vector<zen::data::Raw<double>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<uint8_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<uint16_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<uint32_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<uint64_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<int8_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<int16_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<int32_t>>>();
            register_class<zen::data::Vector<zen::data::IntegerRanged<int64_t>>>();
            register_class<zen::data::Vector<zen::data::FloatRanged<float>>>();
            register_class<zen::data::Vector<zen::data::FloatRanged<double>>>();
        }

        size_t Factory::get_registry_size() const
        {
            return m_type_registry.size();
        }

        Element* Factory::construct_element(Factory::RegistryId registry_id) const
        {
            return m_type_registry[registry_id].m_constructor_func();
        }

        void Factory::destruct_element(Factory::RegistryId registry_id, Element*& element) const
        {
            m_type_registry[registry_id].m_destructor_func(element);
        }

        bool Factory::serialize_registry_id(Factory::RegistryId registry_id, zen::bitstream::Writer& out) const
        {
            return zen::serializers::serialize_integer_ranged(registry_id, Factory::INVALID_REGISTRY_ID, m_registry_id_last, m_registry_id_num_bits, out);
        }

        bool Factory::deserialize_registry_id(Factory::RegistryId& registry_id, zen::bitstream::Reader& in) const
        {
            return zen::serializers::deserialize_integer_ranged(registry_id, Factory::INVALID_REGISTRY_ID, m_registry_id_last, m_registry_id_num_bits, in);
        }
    }
}
