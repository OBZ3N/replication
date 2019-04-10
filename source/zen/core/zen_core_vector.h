#pragma once

#include <cinttypes>
#include <list>
#include <vector>

namespace zen
{
    namespace core
    {
        template<typename TYPE>
        class Vector
        {
        public:
            Vector(size_t capcity = 0);
            virtual ~Vector();

            bool empty() const;
            size_t size() const;
            void reserve(size_t total_elements);

            void push_front(const TYPE& data);
            void push_back(const TYPE& data);

            void pop_front();
            void pop_back();

            void erase(size_t index);
            void insert(size_t index, const TYPE& value);

            TYPE& get(uint32_t index);
            const TYPE& get(uint32_t index) const;

            TYPE& operator[](uint32_t i);
            const TYPE& operator[](uint32_t i) const;

            TYPE& front();
            TYPE& back();

            const TYPE& front() const;
            const TYPE& back() const;

            void clear();
            void sanity_check() const;

            Vector& operator = (const Vector& rhs);

            bool operator == (const Vector& rhs) const;
            bool operator != (const Vector& rhs) const;

        protected:
#undef max
            typedef uint32_t ItemId;
            static const ItemId INVALID_ITEM_ID = (uint32_t)(-1);

            struct Item
            {
                Item()
                    : m_prev(INVALID_ITEM_ID)
                    , m_next(INVALID_ITEM_ID)
                {}

                bool operator == (const Vector::Item& rhs) const;
                bool operator != (const Vector::Item& rhs) const;

                ItemId m_prev;
                ItemId m_next;
                TYPE   m_value;
            };

            void allocate(size_t num_items);
            uint32_t create(const TYPE& value);
            void free(ItemId item_id);

            const Item* get_item(ItemId item_id) const;
            Item* get_item(ItemId item_id);

            std::vector<ItemId>     m_array;
            std::list<ItemId>       m_free;
            std::vector<Item>       m_pool;
        };
    }
}

#include "zen/core/zen_core_vector.hpp"
