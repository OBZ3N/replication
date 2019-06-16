#include "test/test_base.h"

namespace test
{
    Base::Base(const char* test_name, zen::debug::Randomizer& randomizer)
        : m_test_name(test_name)
        , m_randomizer(randomizer)
        , m_state(State::Idle)
        , m_iteration(0)
        , m_test_id(0)
    {}

    Base::~Base()
    {
    }

    Base::State Base::get_state() const
    {
        return m_state;
    }

    void Base::start(uint32_t test_id, uint32_t num_iterations)
    {
        m_test_id = test_id;
        m_iteration = 0;
        m_iteration_limit = num_iterations;
        m_state = State::Running;
        
        ZEN_LOG("[#", m_test_id, ".", m_test_name, "] ", m_iteration_limit, " iterations.");
        start_internal();
    }

    void Base::update()
    {
        m_iteration++;

        ZEN_LOG("[#", m_test_id, ".", m_test_name, "] UPDATE (", m_iteration, " / ", m_iteration_limit, ").");

        if (m_iteration >= m_iteration_limit)
        {
            m_state = State::Success;
            ZEN_LOG("[TEST] SUCCESS.");
        }
        else if (!update_internal())
        {
            m_state = State::Failed;
            ZEN_LOG("[TEST] FAILED.");
        }
    }
}
