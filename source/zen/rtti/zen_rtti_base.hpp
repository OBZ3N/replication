#pragma once

#include "zen/rtti/zen_rtti_base.h"

namespace zen
{
    namespace rtti
    {
        inline Base::~Base()
        {}

        template<typename TYPE>
        inline TYPE& Base::cast_to() const
        {
            ZEN_ASSERT(get_rtti_type_id().is_a(TYPE::get_rtti_type_id()), "base(", rhs.get_type_id().get_name(), ") is not of type(", get_type_id().get_name(), ").");
            
            return static_cast<TYPE&>(*this);
        }

        template<typename TYPE>
        inline const TYPE& Base::cast_to() const
        {
            ZEN_ASSERT(get_rtti_type_id().is_a(TYPE::get_rtti_type_id()), "base(", rhs.get_type_id().get_name(), ") is not of type(", get_type_id().get_name(), ").");
            
            return static_cast<const TYPE&>(*this);
        }
    }
}


