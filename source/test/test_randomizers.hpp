#pragma once

#include "test/test_randomizers.h"
#include "zen/data/zen_data_boolean.h"
#include "zen/data/zen_data_string.h"
#include "zen/data/zen_data_raw.h"
#include "zen/data/zen_data_vector.h"
#include "zen/data/zen_data_component.h"
#include "zen/data/zen_data_float_ranged.h"
#include "zen/data/zen_data_integer_ranged.h"

namespace test
{
    template<>
    inline void randomize_full(zen::data::Boolean& element, zen::debug::Randomizer& randomizer)
    {
        uint32_t i = randomizer.get_integer_ranged(0, 1);
        element.set_value((i == 1));
    }

    template<>
    inline void randomize_delta(zen::data::Boolean& element, const zen::data::Boolean& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}

namespace test
{
    template<>
    inline void randomize_full(zen::data::String& element, zen::debug::Randomizer& randomizer)
    {
        std::string value;
        randomizer.get_string(value, 100);

        element.set_value(value);
    }

    template<>
    inline void randomize_delta(zen::data::String& element, const zen::data::String& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}

namespace test
{
    template<typename TYPE>
    inline void randomize_full(zen::data::Raw<TYPE>& element, zen::debug::Randomizer& randomizer)
    {
        TYPE value;

        randomizer.get_bits(&value, sizeof(value) << 3);

        element.set_value(value);
    }

    template<typename TYPE>
    inline void randomize_delta(zen::data::Raw<TYPE>& element, const zen::data::String& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}

namespace test
{
    template<typename TYPE>
    inline void randomize_full(zen::data::FloatRanged<TYPE>& element, zen::debug::Randomizer& randomizer)
    {
        #undef min
        #undef max
        TYPE type_min = -1.0E6;
        TYPE type_max = 1.0E6;
        TYPE half_range = type_max / 2 - type_min / 2;
        TYPE min = randomizer.get_float_ranged(type_min, half_range);
        TYPE max = randomizer.get_float_ranged(min, (TYPE)(min + half_range));
        TYPE value = randomizer.get_float_ranged(min, max);
        size_t num_bits = randomizer.get_integer_ranged(8, 24);

        element.set_value(value);
        element.set_value_min(min);
        element.set_value_max(max);
        element.set_num_bits(num_bits);
    }

    template<typename TYPE>
    inline void randomize_delta(zen::data::FloatRanged<TYPE>& element, const zen::data::String& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}

namespace test
{
    template<typename TYPE>
    inline void randomize_full(zen::data::IntegerRanged<TYPE>& element, zen::debug::Randomizer& randomizer)
    {
        #undef min
        #undef max
        TYPE type_min = std::numeric_limits<TYPE>::min();
        TYPE type_max = std::numeric_limits<TYPE>::max();
        TYPE half_range = type_max / 2 - type_min / 2;
        TYPE min = randomizer.get_integer_ranged(type_min, (TYPE)(type_min + half_range));
        TYPE max = randomizer.get_integer_ranged(min, (TYPE)(min + half_range - 1));
        TYPE value = randomizer.get_integer_ranged(min, max);

        element.set_value(value);
        element.set_value_min(min);
        element.set_value_max(max);
    }

    template<typename TYPE>
    inline void randomize_delta(zen::data::IntegerRanged<TYPE>& element, const zen::data::String& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}

namespace test
{
    template<typename TYPE>
    inline void randomize_full(zen::data::Vector<TYPE>& element, zen::debug::Randomizer& randomizer)
    {
        element.clear();

        size_t num_items = randomizer.get_integer_ranged<size_t>(8, 100);

        for (size_t i = 0; i < num_items; ++i)
        {
            TYPE item;

            randomize_full(item, randomizer);

            element.push_back(item);
        }
    }

    template<typename TYPE>
    inline void randomize_delta(zen::data::Vector<TYPE>& element, const zen::data::String& reference, zen::debug::Randomizer& randomizer)
    {
        if (randomizer.get_integer_ranged(100) < 20)
        {
            element = reference;
        }
        else
        {
            randomize_full(element, randomizer);
        }
    }
}