#pragma once

#include <stdint.h>

namespace zen
{
    namespace rtti
    {
        class TypeId
        {
        public:
            TypeId(const char* type_name);
            TypeId(const char* type_name, const TypeId& super);

            bool is_a(const TypeId& type_id) const;
            bool operator == (const TypeId& type_id) const;
            bool operator != (const TypeId& type_id) const;

            const char*   get_name() const { return m_name; }
            const TypeId* get_parent() const { return m_parent; }
            const int32_t get_instance_id() const { return m_instance_id;  }

        private:
            static int32_t sm_instance_id_generator;

            const char*     m_name;
            const TypeId*   m_parent;
            int32_t         m_instance_id;
        };
    }
}

#include "zen/rtti/zen_rtti_type_id.hpp"
