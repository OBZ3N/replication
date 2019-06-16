#pragma once

#include <cinttypes>
#include "zen/debug/zen_debug_assert.h"
#include "zen/debug/zen_debug_random.h"

namespace test
{
    class Base
    {
    public:
        enum class State
        {
            Idle,
            Running,
            Success,
            Failed,
        };

        Base(const char* test_name, zen::debug::Randomizer& randomizer);
        virtual ~Base();

        State get_state() const;

        void start(uint32_t test_id, uint32_t num_iterations);
        void update();

    protected:
        virtual void start_internal() =0;
        virtual bool update_internal() = 0;

        const std::string m_test_name;
        zen::debug::Randomizer& m_randomizer;
        State m_state;
        uint32_t m_test_id;
        uint32_t m_iteration;
        uint32_t m_iteration_limit;
    };
}
