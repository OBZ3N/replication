#pragma once

#include "zen/data/zen_data_vector.h"
#include "zen/serializer/zen_serializer_size.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_raw.h"
#include "zen/serializer/zen_serializer_integer_ranged.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Vector<TYPE>::Vector(size_t capacity)
            : core::Vector<TYPE>(capacity)
            , m_index_value_bits(0)
            , m_index_value_max(0)
        {
            on_pool_resized();
        }

        template<typename TYPE>
        Element& Vector<TYPE>::operator = (const Element& rhs)
        {
            const Vector<TYPE>& rhs = (const Vector<TYPE>&) element_rhs;

            clear();

            reserve(rhs.size());

            for (size_t i = 0; i < rhs.size(); ++i)
            {
                push_back(rhs[i]);
            }

            return *this;
        }

        template<typename TYPE>
        bool Vector<TYPE>::serialize_full(bitstream::Writer& out) const
        {
            if (!serializers::serialize_raw(m_array.size(), out))
                return false;
            
            for (ItemId item_id : m_array)
            {
                const Item* item = get_item(item_id);

                serializers::serialize_raw(item_id, out);
                serializers::serialize_raw(item->m_prev, out);
                serializers::serialize_raw(item->m_next, out);
                if (!out.ok())
                    return false;

                if (!item->m_value.serialize_full(out))
                    return false;
            }
            return out.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::deserialize_full(bitstream::Reader& in)
        {
            clear();

            size_t array_size;
            if(!serializers::deserialize_raw(array_size, in))
                return false;

            ItemId head_id = m_array.empty() ? INVALID_ITEM_ID : m_array[0];

            reserve(array_size);

            for (uint32_t i = 0; i < array_size; ++i)
            {
                ItemId item_id;
                ItemId prev;
                ItemId next;
                serializers::deserialize_raw(item_id, in);
                serializers::deserialize_raw(prev, in);
                serializers::deserialize_raw(next, in);
                if (!in.ok()) return false;

                Item* item = get_item(item_id);
                item->m_prev = prev;
                item->m_next = next;
                
                if(!item->m_value.deserialize_full(in))
                    return false;

                if (item->m_prev == INVALID_ITEM_ID)
                    head_id = item_id;

                std::list<ItemId>::iterator it = std::find(m_free.begin(), m_free.end(), item_id);

                ZEN_ASSERT(it != m_free.end(), "item ", item_id, " was added, but couldn't be found in free list");

                m_free.erase(it);
            }

            set_touched(true);

            return in.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::serialize_delta(const Element& element_reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const
        {
            const Vector<TYPE>& reference = (const Vector<TYPE>&) element_reference;

            std::vector<ItemId> items_added;
            std::vector<ItemId> items_removed;
            std::vector<ItemId> items_modified;
            calculate_delta(*this, reference, items_added, items_removed, items_modified);

            bool values_added    = !items_added.empty();
            bool values_removed  = !items_removed.empty();
            bool values_modified = !items_modified.empty();
            bool values_changed  = values_added || values_removed || values_modified;

            serializers::serialize_boolean(values_changed, delta_bits);

            if (values_changed)
            {
                bool index_value_max_changed = (reference.m_index_value_max != m_index_value_max);

                serializers::serialize_boolean(index_value_max_changed, delta_bits);

                if (index_value_max_changed)
                {
                    // pool was resized. All indices ranges will change.
                    serializers::serialize_size(m_index_value_max, out);
                }

                serializers::serialize_boolean(values_added, out);

                if (values_added)
                {
                    serializers::serialize_size(items_added.size(), out);
                    for (ItemId item_id : items_added)
                    {
                        const Item* item = get_item(item_id);
                        serializers::serialize_integer_ranged(item_id,      INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        serializers::serialize_integer_ranged(item->m_prev, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        serializers::serialize_integer_ranged(item->m_next, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        
                        TYPE reference;
                        item->m_value.serialize_delta(reference, out, out);
                    }
                }

                serializers::serialize_boolean(values_removed, out);
                if (values_removed)
                {
                    serializers::serialize_size(items_removed.size(), out);

                    for (ItemId item_id : items_removed)
                    {
                        serializers::serialize_integer_ranged(item_id, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                    }
                }

                serializers::serialize_boolean(values_modified, out);
                if (values_modified)
                {
                    serializers::serialize_size(items_modified.size(), out);

                    for (ItemId item_id : items_modified)
                    {
                        const Item* item_latest = get_item(item_id);
                        
                        bool prev_changed  = (item_latest->m_prev != item_reference->m_prev);
                        bool next_changed  = (item_latest->m_next != item_reference->m_next);
                        bool value_changed = (item_latest->m_value != item_reference->m_value);

                        serializers::serialize_integer_ranged(item_id, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        serializers::serialize_boolean(value_changed, out); // high-frequency.
                        serializers::serialize_boolean(prev_changed, delta_bits); // low-frequency
                        serializers::serialize_boolean(next_changed, delta_bits); // low-frequency

                        if (prev_changed)
                        {
                            serializers::serialize_integer_ranged(item_latest->m_prev, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        }

                        if (next_changed)
                        {
                            serializers::serialize_integer_ranged(item_latest->m_next, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        }

                        if(value_changed)
                        {
                            const Item* item_reference = reference.get_item(item_id);

                            item_latest->m_value.serialize_delta(item_reference->m_value, out);
                        }
                    }
                }
            }

            return out.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::deserialize_delta(const Element& element_reference, bitstream::Reader& in, bitstream::Reader& delta_bits)
        {
            const Vector<TYPE>& reference = (const Vector<TYPE>&) element_reference;
            
            bool value_changed;
            serializers::deserialize_boolean(value_changed, delta_bits);

            if (value_changed)
            {
                ItemId head_id = m_array.empty() ? INVALID_ITEM_ID : m_array[0];

                bool index_value_max_changed;
                serializers::deserialize_boolean(index_value_max_changed, delta_bits);

                if (index_value_max_changed)
                {
                    // pool was resized. All indices ranges will change.
                    serializers::deserialize_size(m_index_value_max, in);
                    m_index_value_bits = zen::serializers::num_bits_required(INVALID_ITEM_ID, m_index_value_max);
                    reserve(m_index_value_max);
                }

                bool values_added;
                serializers::deserialize_boolean(values_added, in);
                if (values_added)
                {
                    size_t items_added;
                    serializers::deserialize_size(items_added, in);
                    if (!in.ok()) return false;
                    
                    for (size_t i = 0; i < items_added; ++i)
                    {
                        ItemId item_id;
                        ItemId prev;
                        ItemId next;
                        serializers::deserialize_integer_ranged(item_id, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        serializers::deserialize_integer_ranged(prev, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);
                        serializers::deserialize_integer_ranged(next, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, out);

                        Item* item = get_item(item_id);
                        item->m_prev = prev;
                        item->m_next = next;

                        TYPE reference;
                        item->m_value.deserialize_delta(reference, in, delta_bits);
                        if (!in.ok()) return false;

                        if (item->m_prev == INVALID_ITEM_ID)
                        {
                            head_id = item_id;
                        }

                        std::list<ItemId>::iterator it = std::find(m_free.begin(), m_free.end(), item_id);

                        ZEN_ASSERT(it != m_free.end(), "item ", item_id, " was added, but couldn't be found in free list");

                        m_free.erase(it);
                    }
                }

                bool values_removed;
                serializers::deserialize_boolean(values_removed, in);
                if (values_removed)
                {
                    size_t items_removed;
                    serializers::deserialize_size(items_removed, in);
                    if (!in.ok()) return false;

                    for (size_t i = 0; i < items_removed; ++i)
                    {
                        ItemId item_id;
                        serializers::deserialize_integer_ranged(item_id, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, in);

                        Item* item = get_item(item_id);

                        if (item_id == head_id)
                        {
                            head_id = INVALID_ITEM_ID;
                        }

                        item->m_prev = INVALID_ITEM_ID;
                        item->m_next = INVALID_ITEM_ID;
                        item->m_value = TYPE();

                        ZEN_ASSERT(std::find(m_free.begin(), m_free.end(), item_id) == m_free.end(), "item ", item_id, " already found in free list");

                        m_free.push_back(item_id);
                    }
                }

                bool values_modified;
                serializers::deserialize_boolean(values_modified, in);
                if (values_modified)
                {
                    size_t items_modified;
                    serializers::deserialize_size(items_modified, in);
                    if (!in.ok()) return false;

                    for (size_t i = 0; i < items_modified; ++i)
                    {
                        ItemId item_id;
                        bool prev_changed;
                        bool next_changed;
                        bool value_changed;

                        serializers::deserialize_integer_ranged(item_id, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, in);
                        serializers::deserialize_boolean(value_changed, in); // high-frequency.
                        serializers::deserialize_boolean(prev_changed, delta_bits); // low-frequency
                        serializers::deserialize_boolean(next_changed, delta_bits); // low-frequency

                        Item* item = get_item(item_id);

                        if (prev_changed)
                        {
                            serializers::deserialize_integer_ranged(item->m_prev, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, in);

                            if (item->m_prev == INVALID_ITEM_ID)
                            {
                                //ZEN_ASSERT( head_id  == INVALID_ITEM_ID, "item ", item_id, " set as head, but head ", head_id, "already found set" );
                                head_id = item_id;
                            }
                        }

                        if (next_changed)
                        {
                            serializers::deserialize_integer_ranged(item->m_next, INVALID_ITEM_ID, m_index_value_max, m_index_value_bits, in);
                        }

                        if (value_changed)
                        {
                            const Item* item_reference = reference.get_item(item_id);

                            item_latest->m_value.serialize_delta(item_reference->m_value, out);
                        }

                        if (!in.ok()) 
                            return false;
                    }
                }

                // rebuild array.
                m_array.clear();

                ItemId item_id = head_id;

                while (item_id != INVALID_ITEM_ID)
                {
                    m_array.push_back(item_id);

                    Item* item = get_item(item_id);

                    item_id = item->m_next;
                }
            }
            return in.ok();
        }

        template<typename TYPE>
        void Vector<TYPE>::calculate_delta(
            const Vector& value,
            const Vector& reference,
            std::vector<ItemId>& items_added,
            std::vector<ItemId>& items_removed,
            std::vector<ItemId>& items_modified)
        {
            for (ItemId item_id : value.m_array)
            {
                std::vector<ItemId>::const_iterator it = std::find(reference.m_array.begin(), reference.m_array.end(), item_id);

                if (it == reference.m_array.end())
                {
                    items_added.push_back(item_id);
                }
                else
                {
                    const Item* item_value = value.get_item(item_id);
                    const Item* item_reference = reference.get_item(item_id);

                    if (*item_value != *item_reference)
                    {
                        items_modified.push_back(item_id);
                    }
                }
            }

            for (ItemId item_id : reference.m_array)
            {
                std::vector<ItemId>::const_iterator it = std::find(value.m_array.begin(), value.m_array.end(), item_id);

                if (it == value.m_array.end())
                {
                    items_removed.push_back(item_id);
                }
            }
        }

        template<typename TYPE>
        bool Vector<TYPE>::operator == (const Element& element_rhs) const
        {
            const Vector<TYPE>& rhs = (const Vector<TYPE>&) element_rhs;

            if (size() != rhs.size())
                return false;

            for (size_t i = 0; i < m_array.size(); ++i)
            {
                ItemId index_lhs = m_array[i];
                ItemId index_rhs = rhs.m_array[i];
                
                if (index_lhs != index_rhs)
                    return false;

                const Item& item_lhs = *get_item(index_lhs);
                const Item& item_rhs = *rhs.get_item(index_rhs);

                if (item_lhs != item_rhs)
                    return false;
            }
            return true;
        }

        template<typename TYPE>
        bool Vector<TYPE>::operator != (const Element& element_rhs) const
        {
            const Vector<TYPE>& rhs = (const Vector<TYPE>&) element_rhs;
            
            return !((*this) == rhs);
        }

        template<typename TYPE>
        void Vector<TYPE>::on_pool_resized()
        {
            m_index_value_max = (ItemId)m_pool.size();
            m_index_value_bits = zen::serializers::num_bits_required(INVALID_ITEM_ID, m_index_value_max);
        }

        template<typename TYPE>
        void Vector<TYPE>::on_vector_touched()
        {
            set_touched(true);
        }
    }
}
