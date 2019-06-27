
#include "zen/debug/zen_debug_assert.h"
#include "test/test_data.h"
#include "test/test_randomizers.h"

namespace test
{
    Data::Data(const zen::data::Factory& factory, zen::debug::Randomizer& randomizer)
        : Base("Data", randomizer)
        , m_factory(factory)
    {
    }

    void Data::start_internal()
    {
    }

    bool Data::update_internal()
    {
        zen::data::Factory::RegistryId registry_id = (zen::data::Factory::RegistryId)m_randomizer.get_integer_ranged(m_factory.get_registry_size());

        zen::data::Element* element_base = m_factory.construct_element(registry_id);

        zen::data::Element* element_server = m_factory.construct_element(registry_id);

        zen::data::Element* element_client = m_factory.construct_element(registry_id);

        randomize_full(*element_base, m_randomizer);

        randomize_full(*element_server, m_randomizer);

        uint32_t m_data[1000];

        zen::bitstream::Writer writer(m_data, sizeof(m_data) << 3);

        ZEN_ASSERT(element_server->serialize_delta(*element_base, writer, writer), "serialize_delta(", m_factory.get_type_id(registry_id)->get_name(), ") failed.");

        zen::bitstream::Reader reader(m_data, writer.bitcount());

        ZEN_ASSERT(element_client->deserialize_delta(*element_base, reader, reader), "deserialize_delta(", m_factory.get_type_id(registry_id)->get_name(), ") failed.");

        ZEN_ASSERT(*element_client == *element_server, "element_client(", m_factory.get_type_id(registry_id)->get_name(), "!=  element_server(", m_factory.get_type_id(registry_id)->get_name(), ").");

        m_factory.destruct_element(registry_id, element_base);

        m_factory.destruct_element(registry_id, element_server);

        m_factory.destruct_element(registry_id, element_client);

        return true;
    }
}
