#pragma once

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class IntegerRanged
        {
        public:
            IntegerRanged(TYPE value = 0, TYPE value_min = 0, TYPE value_max = 0, TYPE value_default = 0);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in);

            bool set_value(TYPE value);
            TYPE get_value() const;

            bool operator == (const IntegerRanged& rhs) const;
            bool operator != (const IntegerRanged& rhs) const;

        private:
            TYPE m_value;
            TYPE m_value_min;
            TYPE m_value_max;
            TYPE m_value_default;
        };
    }
}

#include "zen/data/zen_data_integer_ranged.hpp"
