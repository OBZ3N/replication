#pragma once

#include <cinttypes>
#include "zen/debug/zen_debug_assert.h"
#include "zen/debug/zen_debug_random.h"

namespace test
{
    template<typename TYPE>
    void randomize_full(TYPE& element, zen::debug::Randomizer& randomizer);

    template<typename TYPE>
    void randomize_delta(TYPE& element, const TYPE& reference, zen::debug::Randomizer& randomizer);
}

#include "test/test_randomizers.hpp"