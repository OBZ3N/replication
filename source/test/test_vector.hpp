
#include "zen/debug/zen_debug_assert.h"
#include "test/test_vector.h"

namespace test
{
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    Vector<GENERIC_TYPE, ZEN_TYPE>::Vector(zen::debug::Randomizer& randomizer)
        : Base("Vector", randomizer)
        , m_value_iterator(0)
    {
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    void Vector<GENERIC_TYPE, ZEN_TYPE>::start_internal()
    {
        m_value_iterator = 0;
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::update_internal()
    {
        size_t test_to_run = m_randomizer.get_integer_ranged(6);
        switch (test_to_run)
        {
        default:    return false;
        case 0:     return push_front();
        case 1:     return pop_front();
        case 2:     return push_back();
        case 3:     return pop_back();
        case 4:     return erase();
        case 5:     return insert();
        }
    }
    
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::validate()
    {
        ZEN_ASSERT_RETURN(m_zen_array.size() == m_generic_array.size(), false, "m_zen_array.size (", m_zen_array.size(), ") != m_generic_array_size (", m_generic_array.size(), ")");

        for (size_t t = 0; t < m_zen_array.size(); ++t)
        {
            ZEN_ASSERT_RETURN(m_zen_array[t].get_value() == m_generic_array[i], false, "m_zen_array[", t, "] (", m_zen_array[t].get_value(), ") != m_generic_array[", t, "] (", m_generic_array[t], ")");
        }
        return true;
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::push_front()
    {
        m_value_iterator++;

        ZEN_LOG("    - push_front(", m_value_iterator, ")");

        GENERIC_TYPE generic_value = m_value_iterator;
        m_generic_array.push_front(generic_value);

        ZEN_TYPE zen_value;
        zen_value.set_value(m_value_iterator);
        m_zen_array.push_front(zen_value);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::push_back()
    {
        m_value_iterator++;
        ZEN_LOG("    - push_back(", m_value_iterator, ")");
        
        GENERIC_TYPE generic_value = m_value_iterator;
        m_generic_array.push_back(generic_value);

        ZEN_TYPE zen_value;
        zen_value.set_value(m_value_iterator);
        m_zen_array.push_back(zen_value);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::pop_front()
    {
        if (m_zen_array.empty())
        {
            if (!push_front())
                return false;
        }

        ZEN_LOG("    - pop_front()");

        GENERIC_TYPE generic_value = m_generic_array.pop_front();
        ZEN_TYPE zen_value = m_zen_array.pop_front();

        ZEN_ASSERT_RETURN(generic_value == zen_value.get_value(), false, "generic_value (", generic_value, ") != zen_value (", zen_value.get_value(), ")");

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::pop_back()
    {
        if (m_zen_array.empty())
        {
            if (!push_back())
                return false;
        }

        ZEN_LOG("    - pop_back()");

        GENERIC_TYPE generic_value = m_generic_array.pop_back();
        ZEN_TYPE zen_value = m_zen_array.pop_back();

        ZEN_ASSERT_RETURN(generic_value == zen_value.get_value(), false, "generic_value (", generic_value, ") != zen_value (", zen_value.get_value(), ")");

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::erase()
    {
        if (m_zen_array.empty())
        {
            if (!push_back())
                return false;
        }

        uint32_t index = randomizer.get_integer_ranged(m_zen_array.size());
        ZEN_LOG("    - erase(", index, ")");

        std::list<GENERIC_TYPE>::iterator it = m_generic_array.begin();
        std::advance(it, index);

        m_generic_array.erase(it);
        m_zen_array.erase(index);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::insert()
    {
        m_value_iterator++;
        uint32_t index = randomizer.get_integer_ranged(m_zen_array.size());
        ZEN_LOG("    - insert(", m_value_iterator, " @ ", index, ")");

        std::list<GENERIC_TYPE>::iterator it = m_generic_array.begin();
        std::advance(it, index);
        m_generic_array.insert(it, m_value_iterator);

        ZEN_TYPE zen_value;
        zen_value.set_value(m_value_iterator);
        m_zen_array.insert(index, zen_value);

        return validate();
    }
}
