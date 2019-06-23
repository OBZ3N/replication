#pragma once

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
            const TypeId& get_parent() const { return m_parent; }

        private:
            const char*   m_name;
            const TypeId* m_parent;
        };
    }
}

#include "zen/data/zen_data_rtti.hpp"
