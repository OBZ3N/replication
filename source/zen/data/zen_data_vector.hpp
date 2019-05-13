#pragma once

#include "zen/data/zen_data_vector.h"
#include "zen/serializer/zen_serializer_boolean.h"
#include "zen/serializer/zen_serializer_raw.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Vector<TYPE>::Vector(size_t capacity)
            : core::Vector<TYPE>(capacity)
        {}

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
            bool value_changed   = values_added || values_removed || values_modified;

            if (!serializers::serialize_boolean(value_changed, delta_bits))
                return false;

            if (value_changed)
            {
                if (!serializers::serialize_boolean(values_added, out))
                    return false;

                if (values_added)
                {
                    if (!serializers::serialize_raw(items_added.size(), out))
                        return false;

                    for (ItemId item_id : items_added)
                    {
                        const Item* item = get_item(item_id);
                        serializers::serialize_raw(item_id, out);
                        serializers::serialize_raw(item->m_prev, out);
                        serializers::serialize_raw(item->m_next, out);
                        item->m_value.serialize_full(out);
                        if (!out.ok()) return false;
                    }
                }

                if (!serializers::serialize_boolean(values_removed, out))
                    return false;

                if (values_removed)
                {
                    if (!serializers::serialize_raw(items_removed.size(), out))
                        return false;

                    for (ItemId item_id : items_removed)
                    {
                        if (!serializers::serialize_raw(item_id, out))
                            return false;
                    }
                }

                if (!serializers::serialize_boolean(values_modified, out))
                    return false;

                if (values_modified)
                {
                    if (!serializers::serialize_raw(items_modified.size(), out))
                        return false;

                    for (ItemId item_id : items_modified)
                    {
                        const Item* item_latest = get_item(item_id);
                        const Item* item_reference = reference.get_item(item_id);

                        uint8_t item_flags = 0;
                        if (item_latest->m_prev != item_reference->m_prev) item_flags |= 1;
                        if (item_latest->m_next != item_reference->m_next) item_flags |= 2;
                        if (item_latest->m_value != item_reference->m_value) item_flags |= 4;

                        if (item_flags != 0)
                        {
                            serializers::serialize_raw(item_id, out);
                            serializers::serialize_raw(item_flags, out);
                            if (!out.ok()) return false;

                            if (item_flags & 1)
                            {
                                if (!serializers::serialize_raw(item_latest->m_prev, out))
                                    return false;
                            }
                            if (item_flags & 2)
                            {
                                if (!serializers::serialize_raw(item_latest->m_next, out))
                                    return false;
                            }
                            if (item_flags & 4)
                            {
                                if (!item_latest->m_value.serialize_delta(item_reference->m_value, out))
                                    return false;
                            }
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
            if (!serializers::deserialize_boolean(value_changed, delta_bits))
                return false;

            if (value_changed)
            {
                ItemId head_id = m_array.empty() ? INVALID_ITEM_ID : m_array[0];

                bool values_added;
                if (!serializers::deserialize_boolean(values_added, in))
                    return false;

                if (values_added)
                {
                    size_t items_added;
                    if (!serializers::deserialize_raw(items_added, in))
                        return false;

                    for (size_t i = 0; i < items_added; ++i)
                    {
                        ItemId item_id;
                        ItemId prev;
                        ItemId next;
                        serializers::deserialize_raw(item_id, in);
                        serializers::deserialize_raw(prev, in);
                        serializers::deserialize_raw(next, in);

                        reserve(item_id + 1);

                        Item* item = get_item(item_id);
                        item->m_prev = prev;
                        item->m_next = next;

                        if (!item->m_value.deserialize_full(in))
                            return false;

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
                if (!serializers::deserialize_boolean(values_removed, in))
                    return false;

                if (values_removed)
                {
                    size_t items_removed;
                    if (!serializers::deserialize_raw(items_removed, in))
                        return false;

                    for (size_t i = 0; i < items_removed; ++i)
                    {
                        ItemId item_id;
                        if (!serializers::deserialize_raw(item_id, in))
                            return false;

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
                if (!serializers::deserialize_boolean(values_modified, in))
                    return false;

                if (values_modified)
                {
                    size_t items_modified;
                    if (!serializers::deserialize_raw(items_modified, in))
                        return false;

                    for (size_t i = 0; i < items_modified; ++i)
                    {
                        ItemId item_id;
                        if (!serializers::deserialize_raw(item_id, in))
                            return false;

                        Item* item = get_item(item_id);
                        const Item* item_reference = reference.get_item(item_id);

                        uint8_t item_flags;
                        serializers::deserialize_raw(item_flags, in);

                        if (item_flags & 1)
                        {
                            if (!serializers::deserialize_raw(item->m_prev, in))
                                return false;

                            if (item->m_prev == INVALID_ITEM_ID)
                            {
                                //ZEN_ASSERT( head_id  == INVALID_ITEM_ID, "item ", item_id, " set as head, but head ", head_id, "already found set" );
                                head_id = item_id;
                            }
                        }
                        if (item_flags & 2)
                        {
                            if (!serializers::deserialize_raw(item->m_next, in))
                                return false;
                        }
                        if (item_flags & 4)
                        {
                            if (!item->m_value.deserialize_delta(item_reference->m_value, in))
                                return false;
                        }
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
        void Vector<TYPE>::on_vector_touched()
        {
            set_touched(true);
        }
    }
}
