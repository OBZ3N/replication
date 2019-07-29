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
        void Vector<TYPE>::calculate_item_id_num_bits() const
        {
            m_item_id_num_bits = serializers::number_of_bits_required(INVALID_ITEM_ID, (ItemId)m_pool.size() - 1);

            ZEN_ASSERT(m_item_id_num_bits <= sc_item_id_max_bits, "m_serialize_item_id_num_bits(", m_item_id_num_bits, ") > sc_item_id_max_bits(", sc_item_id_max_bits, ").");
        }

        template<typename TYPE>
        typename Vector<TYPE>::ItemId Vector<TYPE>::calculate_item_id_max() const
        {
            return (ItemId)((1 << m_item_id_num_bits) - 1);
        }

        template<typename TYPE>
        bool Vector<TYPE>::serialize_item_id(const ItemId& item_id, bitstream::Writer& out) const
        {
            return serializers::serialize_integer_ranged(item_id, INVALID_ITEM_ID, calculate_item_id_max(), m_item_id_num_bits, out);
        }

        template<typename TYPE>
        bool Vector<TYPE>::deserialize_item_id(ItemId& item_id, bitstream::Reader& in) const
        {
            return serializers::deserialize_integer_ranged(item_id, INVALID_ITEM_ID, calculate_item_id_max(), m_item_id_num_bits, in);
        }
    }

    namespace data
    {
        template<typename TYPE>
        Vector<TYPE>::Vector(Element* container)
            : Element(container)
            , core::Vector<TYPE>(0)
            , m_item_id_num_bits(0)
        {}

        template<typename TYPE>
        Vector<TYPE>::Vector(size_t capacity, Element* container)
            : Element(container)
            , core::Vector<TYPE>(capacity)
            , m_item_id_num_bits(0)
        {}

        template<typename TYPE>
        Element& Vector<TYPE>::operator = (const Element& element_rhs)
        {
            const Vector<TYPE>& rhs = (const Vector<TYPE>&) element_rhs;

            return (*this).operator=(rhs);
        }

        template<typename TYPE>
        Vector<TYPE>& Vector<TYPE>::operator = (const Vector<TYPE>& rhs)
        {
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
                // encoding the item_ids using the pool size for maximum compression.
                // check if we need to re-calculate the number of compression bits.
                bool size_changed = reference.m_pool.size() != m_pool.size();

                // if the m_item_id_num_bits has never been calculated before, calculate it first.
                if (size_changed || m_item_id_num_bits == 0)
                {
                    calculate_item_id_num_bits();
                }

                // then check if the number of compression bits actually changed.
                bool item_id_num_bits_changed = (reference.m_item_id_num_bits != m_item_id_num_bits);

                serializers::serialize_boolean(item_id_num_bits_changed, delta_bits);

                if (item_id_num_bits_changed)
                {
                    serializers::serialize_integer_ranged(m_item_id_num_bits, (size_t)0, sc_item_id_max_bits, sc_item_id_bitcount, out);
                }

                serializers::serialize_boolean(values_added, out);
                if (values_added)
                {
                    serializers::serialize_size(items_added.size(), out);
                    for (ItemId item_id : items_added)
                    {
                        const Item* item = get_item(item_id);
                        serialize_item_id(item_id, out);
                        serialize_item_id(item->m_prev, out);
                        serialize_item_id(item->m_next, out);
                        
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
                        serialize_item_id(item_id, out);
                    }
                }

                serializers::serialize_boolean(values_modified, out);
                if (values_modified)
                {
                    serializers::serialize_size(items_modified.size(), out);

                    for (ItemId item_id : items_modified)
                    {
                        const Item* item_latest = get_item(item_id);
                        const Item* item_reference = reference.get_item(item_id);

                        bool prev_changed  = (item_latest->m_prev != item_reference->m_prev);
                        bool next_changed  = (item_latest->m_next != item_reference->m_next);
                        bool value_changed = (item_latest->m_value != item_reference->m_value);

                        serialize_item_id(item_id, out);
                        serializers::serialize_boolean(value_changed, out); // high-frequency.
                        serializers::serialize_boolean(prev_changed, delta_bits); // low-frequency
                        serializers::serialize_boolean(next_changed, delta_bits); // low-frequency

                        if (prev_changed)
                        {
                            serialize_item_id(item_latest->m_prev, out);
                        }

                        if (next_changed)
                        {
                            serialize_item_id(item_latest->m_next, out);
                        }

                        if(value_changed)
                        {
                            item_latest->m_value.serialize_delta(item_reference->m_value, out, delta_bits);
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
            
            bool value_changed = false;
            serializers::deserialize_boolean(value_changed, delta_bits);

            *this = reference;

            if (!value_changed)
                return in.ok();

            set_touched(true);

            ItemId head_id = m_array.empty() ? INVALID_ITEM_ID : m_array[0];

            bool item_id_num_bits_changed;
            serializers::deserialize_boolean(item_id_num_bits_changed, delta_bits);

            if (item_id_num_bits_changed)
            {
                serializers::deserialize_integer_ranged(m_item_id_num_bits, (size_t)0, sc_item_id_max_bits, sc_item_id_bitcount, in);
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
                    deserialize_item_id(item_id, in);
                    deserialize_item_id(prev, in);
                    deserialize_item_id(next, in);

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
                    deserialize_item_id(item_id, in);

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

                    deserialize_item_id(item_id, in);
                    serializers::deserialize_boolean(value_changed, in); // high-frequency.
                    serializers::deserialize_boolean(prev_changed, delta_bits); // low-frequency
                    serializers::deserialize_boolean(next_changed, delta_bits); // low-frequency

                    Item* item = get_item(item_id);

                    if (prev_changed)
                    {
                        deserialize_item_id(item->m_prev, in);

                        if (item->m_prev == INVALID_ITEM_ID)
                        {
                            ZEN_ASSERT( head_id  == INVALID_ITEM_ID, "item ", item_id, " set as head, but head ", head_id, "already found set" );
                            head_id = item_id;
                        }
                    }

                    if (next_changed)
                    {
                        deserialize_item_id(item->m_next, in);
                    }

                    if (value_changed)
                    {
                        const Item* item_reference = reference.get_item(item_id);

                            item->m_value.deserialize_delta(item_reference->m_value, in, delta_bits);
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

        template<typename TYPE>
        void Vector<TYPE>::debug_randomize(debug::Randomizer& randomizer, float probability)
        {
            size_t max_iterations = (size_t)(size() * probability);

            size_t total_operations = randomizer.get_integer_ranged(max_iterations);

            for (size_t i = 0; i < total_operations; ++i)
            {
                size_t operation = randomizer.get_integer_ranged(7);

                switch (operation)
                {
                default:
                case 0:
                    {
                        size_t index = randomizer.get_integer_ranged(size());

                        TYPE& item = get(index);

                        item.debug_randomize(randomizer, 1.0f);

                        break;
                    }
                case 1:
                    {
                        TYPE item;

                        item.debug_randomize(randomizer, 1.0f);

                        push_back(item);

                        break;
                    }
                case 2:
                    {
                        TYPE item;

                        item.debug_randomize(randomizer, 1.0f);

                        push_front(item);

                        break;
                    }
                case 3:
                    {
                        pop_back();

                        break;
                    }
                case 4:
                    {
                        pop_front();

                        break;
                    }
                case 5:
                    {
                        TYPE item;

                        item.debug_randomize(randomizer, 1.0f);

                        size_t index = randomizer.get_integer_ranged(size());

                        insert(index, item);

                        break;
                    }
                case 6:
                    {
                        size_t index = randomizer.get_integer_ranged(size());

                        erase(index);

                        break;
                    }
                }
            }
        }
    }
}

