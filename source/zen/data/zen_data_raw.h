#pragma once

#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class Raw : public Element
        {
        public:
            Raw();
            Raw(TYPE value);

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const Raw& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Raw& reference, bitstream::Reader& in);

            bool set_value(TYPE value);
            TYPE get_value() const;

            bool operator == (const Raw& rhs) const;
            bool operator != (const Raw& rhs) const;

            Raw& operator = (const Raw& rhs);

        private:
            TYPE m_value;
        };

        typedef Raw<int8_t> Int8;
        typedef Raw<int16_t> Int16;
        typedef Raw<int32_t> Int32;
        typedef Raw<int64_t> Int64;
        typedef Raw<uint8_t> UInt8;
        typedef Raw<uint16_t> UInt16;
        typedef Raw<uint32_t> UInt32;
        typedef Raw<uint64_t> UInt64;
        typedef Raw<float> Float;
        typedef Raw<double> Double;

    }
}

#include "zen/data/zen_data_raw.hpp"
