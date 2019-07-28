#pragma once

#include "zen/data/zen_data_element.h"

namespace zen
{
    namespace data
    {
        inline Element::Element(Element* container)
            : m_touched(false)
            , m_container(container)
        {}

        inline Element::~Element()
        {}

        inline void Element::set_parent_container(Element* container)
        {
            m_container = container;
        }

        inline Element* Element::get_parent_container()
        {
            return m_container;
        }

        inline const Element* Element::get_parent_container() const
        {
            return m_container;
        }

        inline void Element::set_touched(bool value)
        {
            m_touched = value;

            if (m_container != nullptr)
            {
                m_container->set_touched(true);
            }
        }

        inline bool Element::is_touched() const
        {
            return m_touched;
        }

    }
}
