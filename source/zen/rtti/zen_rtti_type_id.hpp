#pragma once

#include "zen/rtti/zen_rtti_type_id.h"

namespace zen
{
    namespace rtti
    {
        inline TypeId::TypeId(const char* type_name)
            : m_name(type_name)
            , m_parent(nullptr)
            , m_instance_id(sm_instance_id_generator++)
        {}

        inline TypeId::TypeId(const char* type_name, const TypeId& parent)
            : m_name(type_name)
            , m_parent(&parent)
            , m_instance_id(sm_instance_id_generator++)
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

