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

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;
            
            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;

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
