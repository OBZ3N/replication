#pragma once

#include <cinttypes>
#include <list>
#include <vector>

#include "zen/core/zen_core_vector.h"

namespace zen
{
    namespace core
    {
        template<typename TYPE>
        Vector<TYPE>::Vector(size_t capacity = 0)
            : m_array(capacity)
            , m_pool(capacity)
        {
        }

        template<typename TYPE>
        Vector<TYPE>::~Vector()
        {}

        template<typename TYPE>
        bool Vector<TYPE>::empty() const
        {
            return m_array.empty();
        }

        template<typename TYPE>
        size_t Vector<TYPE>::size() const
        {
            return m_array.size();
        }

        template<typename TYPE>
        void Vector<TYPE>::push_front(const TYPE& value)
        {
            ItemId item_id = create(value);
            Item* item = get_item(item_id);

            ItemId front_id = m_array.empty() ? INVALID_ITEM_ID : m_array.front();
            Item* front = get_item(front_id);

            if (front)
            {
                item->m_next = front_id;
                front->m_prev = item_id;
            }

            m_array.insert(std::begin(m_array), item_id);

            on_vector_touched();
        }

        template<typename TYPE>
        void Vector<TYPE>::push_back(const TYPE& value)
        {
            ItemId item_id = create(value);
            Item* item = get_item(item_id);

            ItemId back_id = m_array.empty() ? INVALID_ITEM_ID : m_array.back();
            Item* back = get_item(back_id);

            if (back)
            {
                item->m_prev = back_id;
                back->m_next = item_id;
            }

            m_array.insert(std::end(m_array), item_id);

            on_vector_touched();
        }

        template<typename TYPE>
        void Vector<TYPE>::erase(size_t index)
        {
            std::vector<ItemId>::iterator it = m_array.begin();
            std::advance(it, index);

            ItemId item_id = (*it);
            Item* item = get_item(item_id);
            m_array.erase(it);

            ItemId prev_id = item->m_prev;
            Item* prev = get_item(prev_id);

            ItemId next_id = item->m_next;
            Item* next = get_item(next_id);

            if (prev) prev->m_next = next_id;
            if (next) next->m_prev = prev_id;

            item->m_prev = INVALID_ITEM_ID;
            item->m_next = INVALID_ITEM_ID;

            m_free.push_back(item_id);

            on_vector_touched();
        }

        template<typename TYPE>
        void Vector<TYPE>::insert(size_t index, const TYPE& value)
        {
            ItemId item_id = create(value);
            Item* item = get_item(item_id);

            ItemId prev_id = (index > 0) ? m_array[index - 1] : INVALID_ITEM_ID;
            Item* prev = get_item(prev_id);
            if (prev)
            {
                item->m_prev = prev_id;
                prev->m_next = item_id;
            }

            ItemId next_id = m_array[index];
            Item* next = get_item(next_id);
            if (next)
            {
                item->m_next = next_id;
                next->m_prev = item_id;
            }

            std::vector<ItemId>::iterator it = m_array.begin();
            std::advance(it, index);

            m_array.insert(it, item_id);

            on_vector_touched();
        }

        template<typename TYPE>
        TYPE& Vector<TYPE>::get(uint32_t index)
        {
            ItemId item_id = m_array[index];

            on_vector_touched();

            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        const TYPE& Vector<TYPE>::get(uint32_t index) const
        {
            ItemId item_id = m_array[index];
            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        TYPE& Vector<TYPE>::operator[](uint32_t index)
        {
            ItemId item_id = m_array[index];

            on_vector_touched();

            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        const TYPE& Vector<TYPE>::operator[](uint32_t index) const
        {
            ItemId item_id = m_array[index];
            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        TYPE& Vector<TYPE>::front()
        {
            ItemId item_id = m_array.front();
            
            on_vector_touched();

            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        TYPE& Vector<TYPE>::back()
        {
            ItemId item_id = m_array.back();

            on_vector_touched();

            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        const TYPE& Vector<TYPE>::front() const
        {
            ItemId item_id = m_array.front();
            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        const TYPE& Vector<TYPE>::back() const
        {
            ItemId item_id = m_array.back();
            return get_item(item_id)->m_value;
        }

        template<typename TYPE>
        void Vector<TYPE>::pop_front()
        {
            ItemId item_id = m_array.empty() ? INVALID_ITEM_ID : m_array.front();
            Item* front = get_item(item_id);
            m_array.erase(m_array.begin());

            if (front)
            {
                m_free.push_back(item_id);

                ItemId next_id = front->m_next;
                Item* next = get_item(next_id);

                if (next) next->m_prev = INVALID_ITEM_ID;
                front->m_prev = INVALID_ITEM_ID;
                front->m_next = INVALID_ITEM_ID;
            }

            on_vector_touched();
        }

        template<typename TYPE>
        void Vector<TYPE>::pop_back()
        {
            ItemId item_id = m_array.empty() ? INVALID_ITEM_ID : m_array.back();
            Item* back = get_item(item_id);
            m_array.pop_back();

            if (back)
            {
                m_free.push_back(item_id);

                ItemId prev_id = back->m_prev;
                Item* prev = get_item(prev_id);

                if (prev) prev->m_next = INVALID_ITEM_ID;
                back->m_prev = INVALID_ITEM_ID;
                back->m_next = INVALID_ITEM_ID;
            }

            on_vector_touched();
        }

        template<typename TYPE>
        void Vector<TYPE>::reserve(size_t total_elements)
        {
            if (total_elements > m_pool.size())
            {
                size_t num_elements_required = total_elements - m_pool.size();

                size_t num_elements_to_create = (num_elements_required < 8) ? 8 : num_elements_required + num_elements_required / 2;

                allocate(num_elements_to_create);
            }
        }

        template<typename TYPE>
        void Vector<TYPE>::allocate(size_t num_items)
        {
            size_t i = m_pool.size();
            m_pool.resize(m_pool.size() + num_items);

            for (; i < m_pool.size(); ++i)
            {
                m_free.push_back(i);
            }
        }

        template<typename TYPE>
        typename const Vector<TYPE>::Item* Vector<TYPE>::get_item(ItemId item_id) const
        {
            return item_id == INVALID_ITEM_ID ? nullptr : &m_pool[item_id];
        }

        template<typename TYPE>
        typename Vector<TYPE>::Item* Vector<TYPE>::get_item(ItemId item_id)
        {
            return item_id == INVALID_ITEM_ID ? nullptr : &m_pool[item_id];
        }

        template<typename TYPE>
        typename Vector<TYPE>::ItemId Vector<TYPE>::create(const TYPE& value)
        {
            if (m_free.empty())
            {
                // expand pool by half.
                allocate((m_pool.size() > 8) ? m_pool.size() / 2 : 8);
            }

            ItemId item_id = m_free.front();
            m_free.pop_front();

            Item* item = get_item(item_id);
            item->m_value = value;

            return item_id;
        }

        template<typename TYPE>
        void Vector<TYPE>::free(ItemId item_id)
        {
            Item* item = get_item(item_id);

            item->m_prev = INVALID_ITEM_ID;
            item->m_next = INVALID_ITEM_ID;
            m_free.push_back(item_id);
            return item;
        }

        template<typename TYPE>
        Vector<TYPE>& Vector<TYPE>::operator = (const Vector<TYPE>& rhs)
        {
            m_array = rhs.m_array;
            m_free = rhs.m_free;
            m_pool = rhs.m_pool;
            
            on_vector_touched();
            
            return *this;
        }

        template<typename TYPE>
        bool Vector<TYPE>::operator == (const Vector& rhs) const
        {
            return (m_array == rhs.m_array);
        }

        template<typename TYPE>
        bool Vector<TYPE>::operator != (const Vector& rhs) const
        {
            return !((*this) == rhs);
        }

        template<typename TYPE>
        void Vector<TYPE>::clear()
        {
            m_array.clear();
            m_free.clear();
            m_pool.clear();

            on_vector_touched();
        }

        template<typename TYPE>
        bool Vector<TYPE>::Item::operator == (const Vector<TYPE>::Item& rhs) const
        {
            if (m_prev != rhs.m_prev)
                return false;

            if (m_next != rhs.m_next)
                return false;

            if (m_value != rhs.m_value)
                return false;

            return true;
        }

        template<typename TYPE>
        bool Vector<TYPE>::Item::operator != (const Vector<TYPE>::Item& rhs) const
        {
            return !(*this == rhs);
        }

        template<typename TYPE>
        void Vector<TYPE>::sanity_check() const
        {
            if (!m_array.empty())
            {
                ZEN_ASSERT(get_item(m_array.front())->m_prev == INVALID_ITEM_ID, "m_array.front()->m_prev is not invalid (", get_item(m_array.front())->m_prev, ")");
                ZEN_ASSERT(get_item(m_array.back())->m_next == INVALID_ITEM_ID, "m_array.back()->m_next is not invalid (", get_item(m_array.back())->m_next, ")");

                for (size_t i = 0; i < m_array.size() - 1; ++i)
                {
                    ZEN_ASSERT(get_item(m_array[i])->m_next == m_array[i + 1], "item[", i, "]->m_next(", get_item(m_array[i])->m_next, ") != item[", i + 1, "](", m_array[i + 1], ")");
                }

                for (size_t i = 1; i < m_array.size(); ++i)
                {
                    ZEN_ASSERT(get_item(m_array[i])->m_prev == m_array[i - 1], "item[", i, "]->m_prev(", get_item(m_array[i])->m_prev, ") != item[", i - 1, "](", m_array[i - 1], ")");
                }

                for (ItemId item_id : m_array)
                {
                    ZEN_ASSERT(item_id < m_pool.size(), "item[", item_id, "] not in pool");
                }
            }

            for (ItemId item_id : m_free)
            {
                ZEN_ASSERT(item_id < m_pool.size(), "free item[", item_id, "] not in pool");
            }

            for (ItemId item_id : m_free)
            {
                std::vector<ItemId>::const_iterator it = std::find(m_array.begin(), m_array.end(), item_id);
                ZEN_ASSERT(it == m_array.end(), "free item (", item_id, ") found in array.");
            }

            for (ItemId item_id = 0; item_id < (ItemId)m_pool.size(); ++item_id)
            {
                std::vector<ItemId>::const_iterator it = std::find(m_array.begin(), m_array.end(), item_id);
                std::list<ItemId>::const_iterator jt = std::find(m_free.begin(), m_free.end(), item_id);
                ZEN_ASSERT(it != m_array.end() || jt != m_free.end(), "pool item (", item_id, ") not found in array or free list.");
            }
        }
    }
}