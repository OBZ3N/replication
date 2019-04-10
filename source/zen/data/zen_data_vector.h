#pragma once

#include <istream>
#include <ostream>
#include "zen/core/zen_core_vector.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class Vector: public core::Vector<TYPE>
        {
        public:
            Vector(size_t capcity = 0);

            bool serialize_value(bitstream::Writer& out) const;
            bool deserialize_value(bitstream::Reader& in);
            bool serialize_delta(const Vector<TYPE>& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Vector<TYPE>& reference, bitstream::Reader& in);

        private:
            static void calculate_delta(
                const Vector& value,
                const Vector& reference,
                std::vector<ItemId>& items_added,
                std::vector<ItemId>& items_removed,
                std::vector<ItemId>& items_modified);
        };
    }
}
#include "zen/data/zen_data_vector.hpp"