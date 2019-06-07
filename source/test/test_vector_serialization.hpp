
#include "zen/debug/zen_debug_assert.h"
#include "test/test_vector_serialization.h"

namespace test
{
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    VectorSerialization<GENERIC_TYPE, ZEN_TYPE>::VectorSerialization(zen::debug::Randomizer& randomizer)
        : Base("VectorSerialization", randomizer)
    {
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    void VectorSerialization<GENERIC_TYPE, ZEN_TYPE>::start_internal()
    {
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool VectorSerialization<GENERIC_TYPE, ZEN_TYPE>::update_internal()
    {
        return false;
    }
    
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool VectorSerialization<GENERIC_TYPE, ZEN_TYPE>::validate()
    {
        ZEN_ASSERT_RETURN(m_zen_array.size() == m_generic_array.size(), false, "m_zen_array.size (", m_zen_array.size(), ") != m_generic_array_size (", m_generic_array.size(), ")");

        for (size_t t = 0; t < m_zen_array.size(); ++t)
        {
            ZEN_ASSERT_RETURN(m_zen_array[t].get_value() == m_generic_array[i], false, "m_zen_array[", t, "] (", m_zen_array[t].get_value(), ") != m_generic_array[", t, "] (", m_generic_array[t], ")");
        }
        return true;
    }
}
