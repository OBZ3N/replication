#pragma once

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class FloatRanged
        {
        public:
            FloatRanged(TYPE value=0, TYPE value_min=0, TYPE value_max=0, size_t num_bits = 0, TYPE value_default=0);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const FloatRanged& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const FloatRanged& reference, bitstream::Reader& in);

            bool set_value(TYPE value);
            TYPE get_value() const;

            bool operator == (const FloatRanged& rhs) const;
            bool operator != (const FloatRanged& rhs) const;

        private:
            TYPE m_value;
            TYPE m_value_min;
            TYPE m_value_max;
            TYPE m_value_default;
            size_t m_num_bits;
        };
    }
}

#include "zen/data/zen_data_float_ranged.hpp"
