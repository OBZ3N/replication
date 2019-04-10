
#pragma once

#include "zen/data/zen_data_value.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Value<TYPE>::Value()
        {}

        template<typename TYPE>
        Value<TYPE>::Value(TYPE value)
            : m_value(value)
        {}

        template<typename TYPE>
        Value<TYPE>::~Value()
        {}

        template<typename TYPE>
        Value<TYPE>& Value<TYPE>::operator = (const Value& rhs)
        {
            m_value = rhs.m_value;
            return *this;
        }

        template<typename TYPE>
        Value<TYPE>::operator TYPE() const
        {
            return m_value;
        }

        template<typename TYPE>
        Value<TYPE>::operator TYPE&()
        {
            return m_value;
        }

        template<typename TYPE>
        bool Value<TYPE>::operator == (const Value& rhs)
        {
            return (m_value == rhs.m_value);
        }

        template<typename TYPE>
        bool Value<TYPE>::operator != (const Value& rhs)
        {
            return (m_value != rhs.m_value);
        }
    }
}
