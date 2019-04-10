#pragma once

#include "zen/data/zen_data_float.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE, TYPE value_default>
        Float<TYPE, value_default>::Float(TYPE value)
            : Value<TYPE>(value)
        {}

        template<typename TYPE, TYPE value_default>
        bool Float<TYPE, value_default>::serialize_delta(const Float& reference, bitstream::Writer& out) const
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                out,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Float<TYPE>());
            return out.ok();
        }

        template<typename TYPE, TYPE value_default>
        bool Float<TYPE, value_default>::deserialize_delta(const Float& reference, bitstream::Reader& in)
        {
            zen::serializers::serialize_delta(
                m_value,
                reference.m_value,
                in,
                zen::serializers::Diff<TYPE>(),
                zen::serializers::Default<TYPE, value_default>(),
                zen::serializers::Float<TYPE>());
            return in.ok();
        }
    }
}

#include "zen/data/zen_data_float.hpp"
