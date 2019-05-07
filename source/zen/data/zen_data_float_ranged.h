#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class FloatRanged : public Element
        {
        public:
            FloatRanged();
            FloatRanged(TYPE value, TYPE value_min, TYPE value_max, size_t num_bits);

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const FloatRanged& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const FloatRanged& reference, bitstream::Reader& in);

            bool set_value(TYPE value);
            TYPE get_value() const;

            bool set_value_min(TYPE value_min);
            TYPE get_value_min() const;

            bool set_value_max(TYPE value_max);
            TYPE get_value_max() const;

            bool set_num_bits(size_t num_bits);
            size_t get_num_bits() const;

            bool operator == (const FloatRanged& rhs) const;
            bool operator != (const FloatRanged& rhs) const;

            inline FloatRanged& operator = (const FloatRanged& rhs);

        private:
            TYPE m_value;
            TYPE m_value_min;
            TYPE m_value_max;
            size_t m_num_bits;
        };
    }
}

#include "zen/data/zen_data_float_ranged.hpp"
