#pragma once

#include "zen/debug/zen_debug_random.h"
#include "zen/rtti/zen_rtti_base.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace data
    {
        class Element : public zen::rtti::Base
        {
        public:
            DECLARE_RTTI_TYPE_ID();

            Element(Element* container);
            virtual ~Element();

            void set_parent_container(Element* container);
            Element* get_parent_container();
            const Element* get_parent_container() const;

            void set_touched(bool value);
            bool is_touched() const;

            virtual bool serialize_full(bitstream::Writer& out) const = 0;
            virtual bool deserialize_full(bitstream::Reader& in) = 0;

            virtual bool serialize_delta(const Element& reference, bitstream::Writer& out, bitstream::Writer& delta_bits) const = 0;
            virtual bool deserialize_delta(const Element& reference, bitstream::Reader& in, bitstream::Reader& delta_bits) = 0;

            virtual bool operator == (const Element& rhs) const = 0;
            virtual bool operator != (const Element& rhs) const = 0;

            virtual Element& operator = (const Element& rhs) = 0;

            virtual void debug_randomize_full(debug::Randomizer& randomizer) = 0;
            virtual void debug_randomize_delta(const Element& reference, debug::Randomizer& randomizer) = 0;

        protected:
            bool m_touched;
            Element* m_container;
        };
    }
}

#include "zen/data/zen_data_element.hpp"
