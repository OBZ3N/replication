#pragma once

#include "zen/data/zen_data_element.h"

namespace zen
{
    namespace data
    {
        inline Element::Element()
            : m_touched(false)
        {}

        inline Element::~Element()
        {}

        inline void Element::set_touched(bool value)
        {
            m_touched = value;
        }

        inline bool Element::is_touched() const
        {
            return m_touched;
        }

    }
}
