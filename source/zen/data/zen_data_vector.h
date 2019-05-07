#pragma once

#include "zen/core/zen_core_vector.h"
#include "zen/data/zen_data_element.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        class Vector: public Element, public core::Vector<TYPE>
        {
        public:
            Vector(size_t capcity = 0);

            bool serialize_full(bitstream::Writer& out) const;
            bool deserialize_full(bitstream::Reader& in);
            bool serialize_delta(const Vector& reference, bitstream::Writer& out) const;
            bool deserialize_delta(const Vector& reference, bitstream::Reader& in);
            
            inline bool operator == (const Vector& rhs) const;
            inline bool operator != (const Vector& rhs) const;

            Vector& operator = (const Vector& rhs);

        protected:
            void on_vector_touched() override;

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
