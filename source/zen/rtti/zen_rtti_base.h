#pragma once

#include "zen/rtti/zen_rtti_type_id.h"

#define DECLARE_RTTI_TYPE_ID_BASE()                                 \
static zen::rtti::TypeId s_rtti_type_id_instance;                   \
static const zen::rtti::TypeId& get_type_id_instance(void)                 \
{                                                                   \
    return s_rtti_type_id_instance;                                 \
}                                                                   \
                                                                    \
inline virtual const zen::rtti::TypeId& get_type_id(void) const     \
{                                                                   \
    return s_rtti_type_id_instance;                                 \
}                                                                   \

#define DECLARE_RTTI_TYPE_ID()                                      \
static zen::rtti::TypeId s_rtti_type_id_instance;                   \
static const zen::rtti::TypeId& get_type_id_instance(void)                 \
{                                                                   \
    return s_rtti_type_id_instance;                                 \
}                                                                   \
                                                                    \
inline const zen::rtti::TypeId& get_type_id(void) const override    \
{                                                                   \
    return s_rtti_type_id_instance;                                 \
}                                                                   \

#define IMPLEMENT_RTTI_TYPE_ID_BASE( TYPE )              \
zen::rtti::TypeId TYPE::s_rtti_type_id_instance(#TYPE);  \

#define IMPLEMENT_RTTI_TYPE_ID( TYPE, TYPE_PARENT )                                             \
zen::rtti::TypeId TYPE::s_rtti_type_id_instance(#TYPE, TYPE_PARENT::s_rtti_type_id_instance);   \

namespace zen
{

    namespace rtti
    {
        class Base
        {
        public:
            DECLARE_RTTI_TYPE_ID_BASE();

            virtual ~Base();

            template<typename TYPE> inline TYPE& cast_to() const;

            template<typename TYPE> inline const TYPE& cast_to() const;
        };
    }
}

#include "zen/rtti/zen_rtti_base.hpp"