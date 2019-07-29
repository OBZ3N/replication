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
            DECLARE_RTTI_TYPE_ID();

            Vector(Element* container = nullptr);
            Vector(size_t capcity, Element* container = nullptr);

            bool serialize_full(bitstream::Writer& out) const override;
            bool deserialize_full(bitstream::Reader& in) override;

            bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const override;
            bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) override;
            
            inline bool operator == (const Element& rhs) const override;
            inline bool operator != (const Element& rhs) const override;

            Element& operator = (const Element& rhs) override;
            Vector<TYPE>& operator = (const Vector<TYPE>& rhs);

            void debug_randomize(debug::Randomizer& randomizer, float probability) override;

        protected:
            void on_vector_touched() override;

        private:
            // calculate the difference in two vectors,
            // and building lists of items to add, items to remove, and items that have been modified.
            static void calculate_delta(
                const Vector& value,
                const Vector& reference,
                std::vector<ItemId>& items_added,
                std::vector<ItemId>& items_removed,
                std::vector<ItemId>& items_modified);

            //-----------------------------------------------------------------------------
            // compressing item id's to number of bits required to serialize them.
            // this will optimize the delta compression of items that are being added, removed, or modified.
            //-----------------------------------------------------------------------------
        private:
            // i.e. item ids cannot be greater than the actual item pool size, 
            // or lower than INVALID_ITEM_ID, which is (-1).
            // And item ids cannot require more than 31 bits (cannot have vectors of size > 2^31 elements).
            static constexpr size_t sc_item_id_bitcount = 5;

            static constexpr size_t sc_item_id_max_bits = ((1 << sc_item_id_bitcount)-1);

            // number of bits required to serialize item ids (the number of bits required to serialize the range if ids [INVALID_ITEM_ID, m_pool.size()[).
            void calculate_item_id_num_bits() const;

            // maximum value an item id can have ((1 << m_item_id_num_bits) - 1), which is the power of two higher than m_pool.size().
            ItemId calculate_item_id_max() const;

            // helpers that will serialize / deserialize item ids.
            bool serialize_item_id(const ItemId& item_id, bitstream::Writer& out) const;
            bool deserialize_item_id(ItemId& item_id, bitstream::Reader& in) const;

            // number of bits required to serialize item ids (roughly speaking, we round up to the power of two higher than the pool size, not forgetting the value INVALID_ITEM_ID).
            mutable size_t m_item_id_num_bits;
        };
    }
}
#include "zen/data/zen_data_vector.hpp"
