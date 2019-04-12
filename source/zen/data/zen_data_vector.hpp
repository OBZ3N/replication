#pragma once

#include "./zen_data_serializers.h"
#include "./zen_data_vector.h"

namespace zen
{
    namespace data
    {
        template<typename TYPE>
        Vector<TYPE>::Vector(size_t capacity)
            : core::Vector<TYPE>(capacity)
        {}

        template<typename TYPE>
        bool Vector<TYPE>::serialize_value(bitstream::Writer& out) const
        {
            if (!serializers::serialize_raw(m_array.size()))
                return false;
            
            for (ItemId item_id : m_array)
            {
                const Item* item = get_item(item_id);

                serializers::serialize_raw(item_id, out);
                serializers::serialize_raw(item->m_prev, out);
                serializers::serialize_raw(item->m_next, out);
                if (!out.ok())
                    return false;

                if (!item->m_value.serialize_value(out, nullptr))
                    return false;
            }
            return out.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::deserialize_value(bitstream::Reader& in)
        {
            clear();

            size_t array_size;
            if(!serializers::deserialize_raw(array_size))
                return false;

            m_value.reserve(array_size);

            for (uint32_t i = 0; i < array_size; ++i)
            {
                ItemId item_id;
                ItemId prev;
                ItemId next;
                serializers::deserialize_raw(item_id);
                serializers::deserialize_raw(prev);
                serializers::deserialize_raw(next);
                if (!in.ok()) return false;

                Item* item = get_item(item_id);
                item->m_prev = prev;
                item->m_next = next;
                
                if(!item->m_value.deserialize_value(in))
                    return false;

                if (item->m_prev == INVALID_ITEM_ID)
                    head_id = item_id;

                std::list<ItemId>::iterator it = std::find(m_free.begin(), m_free.end(), item_id);

                ZEN_ASSERT(it != m_free.end(), "item ", item_id, " was added, but couldn't be found in free list");

                m_free.erase(it);
            }
            return in.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::serialize_delta(const Vector<TYPE>& reference, bitstream::Writer& out) const
        {
            std::vector<ItemId> items_added;
            std::vector<ItemId> items_removed;
            std::vector<ItemId> items_modified;
            calculate_delta(*this, reference, items_added, items_removed, items_modified);

            // no change.
            if (items_added.empty() &&
                items_removed.empty() &&
                items_modified.empty())
            {
                return false;
            }

            uint8_t flags = 0;
            if (!items_added.empty()) flags |= 1;
            if (!items_removed.empty()) flags |= 2;
            if (!items_modified.empty()) flags |= 4;

            // serialize header.
            if(!serializers::serialize_raw(flags, out))
                return false;

            if (!items_added.empty())
            {
                if(!serializers::serialize_raw(items_added.size(), out))
                    return false;

                for (ItemId item_id : items_added)
                {
                    const Item* item = get_item(item_id);
                    serializers::serialize_raw(item_id, out);
                    serializers::serialize_raw(item->m_prev, out);
                    serializers::serialize_raw(item->m_next, out);
                    item->m_value.serialize_value(out);
                    if (!out.ok()) return false;
                }
            }

            if (!items_removed.empty())
            {
                if (!serializers::serialize_raw(items_removed.size(), out))
                    return false;

                for (ItemId item_id : items_removed)
                {
                    if (!serializers::serialize_raw(item_id, out))
                        return false;
                }
            }

            if (!items_modified.empty())
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
            return out.ok();
        }

        template<typename TYPE>
        bool Vector<TYPE>::deserialize_delta(const Vector<TYPE>& reference, bitstream::Reader& in)
        {
            uint8_t flags = 0;
            if (!serializers::deserialize_raw(flags, in))
                return false;

            ItemId head_id = m_array.empty() ? INVALID_ITEM_ID : m_array[0];

            if (flags & 1)
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
                    
                    if (!item->m_value.deserialize_value(in))
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

            if (flags & 2)
            {
                size_t items_removed;
                if(!serializers::deserialize_raw(items_removed, in))
                    return false;

                for (size_t i = 0; i < items_removed; ++i)
                {
                    ItemId item_id;
                    if(!serializers::deserialize_raw(item_id, in))
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

            if (flags & 4)
            {
                size_t items_modified;
                if(!serializers::deserialize_raw(items_modified, in))
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
    }
}
