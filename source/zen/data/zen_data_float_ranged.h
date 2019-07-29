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
            DECLARE_RTTI_TYPE_ID();

            FloatRanged(Element* container = nullptr);
            FloatRanged(TYPE value, TYPE value_min, TYPE value_max, size_t num_bits, Element* container = nullptr);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;

            bool operator == (const FloatRanged<TYPE>& rhs) const;
            bool operator == (const Element& rhs) const override;
            bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;
            FloatRanged<TYPE>& operator = (const FloatRanged<TYPE>& rhs);
            
            bool set_value(TYPE value);
            TYPE get_value() const;

            bool set_value_min(TYPE value_min);
            TYPE get_value_min() const;

            bool set_value_max(TYPE value_max);
            TYPE get_value_max() const;

            bool set_num_bits(size_t num_bits);
            size_t get_num_bits() const;

            void debug_randomize(debug::Randomizer& randomizer, float probability) override;

        private:
            TYPE calculate_precision() const;
            TYPE m_value;
            TYPE m_value_min;
            TYPE m_value_max;
            TYPE m_precision;
            size_t m_num_bits;
        };
    }
}

#include "zen/data/zen_data_float_ranged.hpp"
