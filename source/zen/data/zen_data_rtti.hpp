#pragma once

#include "zen/data/zen_data_rtti.h"

namespace zen
{
    namespace rtti
    {
        inline TypeId::TypeId(const char* type_name)
            : m_name(type_name)
            , m_parent(nullptr)
        {}

        inline TypeId::TypeId(const char* type_name, const TypeId& parent)
            : m_name(type_name)
            , m_parent(&parent)
        {}

        inline bool TypeId::is_a(const TypeId& rhs) const
        {
            const TypeId* p = this;

            do
            {
                if (p == &rhs)
                    return true;

                p = p->m_parent;

                if (p == nullptr)
                    return false;
            } while (1);

            return false;
        }

        inline bool TypeId::operator == (const TypeId& rhs) const
        {
            return this == &rhs;
        }

        inline bool TypeId::operator != (const TypeId& rhs) const
        {
            return this != &rhs;
        }
    }
}

