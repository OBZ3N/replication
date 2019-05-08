#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class IntegerRanged : public Element
        {
        public:
            IntegerRanged();
            IntegerRanged(TYPE value, TYPE value_min, TYPE value_max);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;
            bool serialize_delta(const IntegerRanged& reference, bitstream::Writer& out) const override;
            bool deserialize_delta(const IntegerRanged& reference, bitstream::Reader& in) override;

            bool set_value(TYPE value);
            TYPE get_value() const;

            bool set_value_min(TYPE value_min);
            TYPE get_value_min() const;

            bool set_value_max(TYPE value_max);
            TYPE get_value_max() const;

            bool operator == (const IntegerRanged& rhs) const;
            bool operator != (const IntegerRanged& rhs) const;

            IntegerRanged& operator = (const IntegerRanged& rhs);

        private:
            TYPE    m_value;
            TYPE    m_value_min;
            TYPE    m_value_max;
            size_t  m_num_bits;
        };
    }
}

#include "zen/data/zen_data_integer_ranged.hpp"
