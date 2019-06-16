
#include "zen/debug/zen_debug_assert.h"
#include "test/test_vector.h"

namespace test
{
    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    Vector<GENERIC_TYPE, ZEN_TYPE>::Vector(zen::debug::Randomizer& randomizer)
        : Base("Vector", randomizer)
    {
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    void Vector<GENERIC_TYPE, ZEN_TYPE>::start_internal()
    {
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    void Vector<GENERIC_TYPE, ZEN_TYPE>::calculate_valid_tests(std::vector<Vector<GENERIC_TYPE, ZEN_TYPE>::Test>& tests)
    {
        tests.clear();
        tests.push_back(Test::PushFront);
        tests.push_back(Test::PushBack);

        if (m_generic_array.size() > 0)
        {
            tests.push_back(Test::Insert);
        }

        if (m_generic_array.size() > 8)
        {
            tests.push_back(Test::PopFront);
            tests.push_back(Test::PopBack);
            tests.push_back(Test::Erase);
        }
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::update_internal()
    {
        std::vector<Test> tests;
        calculate_valid_tests(tests);

        size_t test_to_run = m_randomizer.get_integer_ranged(tests.size());
        switch (tests[test_to_run])
        {
        default:                return false;
        case Test::PushFront:   return push_front();
        case Test::PopFront:    return pop_front();
        case Test::PushBack:    return push_back();
        case Test::PopBack:     return pop_back();
        case Test::Erase:       return erase();
        case Test::Insert:      return insert();
        }
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::validate()
    {
        ZEN_ASSERT_RETURN(m_zen_array.size() == m_generic_array.size(), false, "m_zen_array.size (", m_zen_array.size(), ") != m_generic_array_size (", m_generic_array.size(), ")");

        for (size_t t = 0; t < m_zen_array.size(); ++t)
        {
            ZEN_ASSERT_RETURN(m_zen_array[t].get_value() == m_generic_array[t], false, "m_zen_array[", t, "] (", m_zen_array[t].get_value(), ") != m_generic_array[", t, "] (", m_generic_array[t], ")");
        }
        return true;
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::push_front()
    {
        ZEN_TYPE zen_value;
        zen_value.debug_randomize(m_randomizer);

        ZEN_LOG("    - push_front(", zen_value.get_value(), ")");

        m_generic_array.insert(m_generic_array.begin(), zen_value.get_value());
        m_zen_array.push_front(zen_value);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::push_back()
    {
        ZEN_TYPE zen_value;
        zen_value.debug_randomize(m_randomizer);

        ZEN_LOG("    - push_back(", zen_value.get_value(), ")");

        m_generic_array.push_back(zen_value.get_value());
        m_zen_array.push_back(zen_value);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::pop_front()
    {
        ZEN_LOG("    - pop_front()");

        GENERIC_TYPE generic_value = m_generic_array.front();
        m_generic_array.erase(m_generic_array.begin());

        ZEN_TYPE zen_value = m_zen_array.front();
        m_zen_array.pop_front();

        ZEN_ASSERT_RETURN(generic_value == zen_value.get_value(), false, "generic_value (", generic_value, ") != zen_value (", zen_value.get_value(), ")");

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::pop_back()
    {
        ZEN_LOG("    - pop_back()");

        GENERIC_TYPE generic_value = m_generic_array.back();
        m_generic_array.pop_back();

        ZEN_TYPE zen_value = m_zen_array.back();
        m_zen_array.pop_back();

        ZEN_ASSERT_RETURN(generic_value == zen_value.get_value(), false, "generic_value (", generic_value, ") != zen_value (", zen_value.get_value(), ")");

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::erase()
    {
        uint32_t index = m_randomizer.get_integer_ranged(m_zen_array.size());
        ZEN_LOG("    - erase(", index, ")");

        std::vector<GENERIC_TYPE>::iterator it = m_generic_array.begin();
        std::advance(it, index);

        m_generic_array.erase(it);
        m_zen_array.erase(index);

        return validate();
    }

    template<typename GENERIC_TYPE, typename ZEN_TYPE>
    bool Vector<GENERIC_TYPE, ZEN_TYPE>::insert()
    {
        ZEN_TYPE zen_value;
        zen_value.debug_randomize(m_randomizer);

        uint32_t index = m_randomizer.get_integer_ranged(m_zen_array.size());
        std::vector<GENERIC_TYPE>::iterator it = m_generic_array.begin();
        std::advance(it, index);
        
        ZEN_LOG("    - insert(", zen_value.get_value(), " @ ", index, ")");

        m_generic_array.insert(it, zen_value.get_value());
        m_zen_array.insert(index, zen_value);

        return validate();
    }
}
