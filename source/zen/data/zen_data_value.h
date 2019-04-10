#pragma once

#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"
#include "zen/data/zen_data_serializers.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class Value
        {
        public:
            Value();

            Value(TYPE value);

            ~Value();

            operator TYPE() const;

            operator TYPE&();

            const TYPE& value() const { return m_value; }

            TYPE& value() { return m_value; }

            Value& operator = (const Value& rhs);

            bool operator == (const Value& rhs);

            bool operator != (const Value& rhs);

        protected:
            TYPE m_value;
        };
    }
}

#include "zen/data/zen_data_value.hpp"
