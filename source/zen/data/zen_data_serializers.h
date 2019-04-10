#pragma once

#include <string>
#include "zen/debug/zen_debug_assert.h"
#include "zen/bitstream/zen_bitstream_reader.h"
#include "zen/bitstream/zen_bitstream_writer.h"

namespace zen
{
    namespace serializers
    {
        template <size_t N> struct Log2 { static size_t get() { return 1 + Log2<N / 2>::value; } };
        template <> struct Log2<0> { static size_t get() { ZEN_ASSERT(false, "invalid call to Log2<0>"); } };
        template <> struct Log2<1> { static size_t get() { return 0; } };

        extern bool serialize_boolean(bool value, bitstream::Writer& out);
            
        extern bool deserialize_boolean(bool& value, bitstream::Reader& in);

        extern bool serialize_string(const std::string& value, bitstream::Writer& out);

        extern bool deserialize_string(std::string& value, bitstream::Reader& in);

        template<typename TYPE>
        bool serialize_raw(const TYPE& value, bitstream::Writer& out);

        template<typename TYPE>
        bool deserialize_raw(TYPE& value, bitstream::Reader& in);

        template<typename TYPE>
        bool serialize_float_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Writer& out);
        
        template<typename TYPE>
        bool deserialize_float_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Reader& in);

        template<typename TYPE>
        bool serialize_integer_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, bitstream::Writer& out);

        template<typename TYPE>
        bool deserialize_integer_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, bitstream::Reader& in);

        template<typename TYPE>
        class Diff
        {
        public:
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE, TYPE value_default>
        class Default
        {
        public:
            bool serialize_value(const TYPE& value, bitstream::Writer& out) const;
            bool deserialize_value(TYPE& value, bitstream::Reader& in) const;
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        class Boolean
        {
        public:
            bool serialize_value(const bool& value, bitstream::Writer& out) const;
            bool deserialize_value(bool& value, bitstream::Reader& in) const;
            bool serialize_delta(const bool& value, const bool& reference, bitstream::Writer& out) const;
            bool deserialize_delta(bool& value, const bool& reference, bitstream::Reader& in) const;
        };

        class String
        {
        public:
            bool serialize_value(const std::string& value, bitstream::Writer& out) const;
            bool deserialize_value(std::string& value, bitstream::Reader& in) const;
            bool serialize_delta(const std::string& str, const std::string& reference, bitstream::Writer& out) const;
            bool deserialize_delta(std::string& str, const std::string& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE>
        class Integer
        {
        public:
            bool serialize_value(const TYPE& value, bitstream::Writer& out) const;
            bool deserialize_value(TYPE& value, bitstream::Reader& in) const;
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE>
        class Float
        {
        public:
            bool serialize_value(const TYPE& value, bitstream::Writer& out) const;
            bool deserialize_value(TYPE& value, bitstream::Reader& in) const;
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE, TYPE value_min, TYPE value_max>
        class IntegerRanged
        {
        public:
            bool serialize_value(const TYPE& value, bitstream::Writer& out) const;
            bool deserialize_value(TYPE& value, bitstream::Reader& in) const;
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits>
        class FloatRanged
        {
        public:
            bool serialize_value(const TYPE& value, bitstream::Writer& out) const;
            bool deserialize_value(TYPE& value, bitstream::Reader& in) const;
            bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const;
            bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const;
        };

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool serialize_value(const TYPE& value_to_serialise, bitstream::Writer& out, const FirstSerializer& first_serialiser, const NextSerialisers&... next_serializers);

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool deserialize_value(TYPE& value_to_deserialise, bitstream::Reader& in, const FirstSerializer& first_serialiser, const NextSerialisers&... next_serializers);

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out, const FirstSerializer& first_serialiser, const NextSerialisers&... next_serializers);

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in, const FirstSerializer& first_serialiser, const NextSerialisers&... next_serializers);
    }
}

#include "zen/data/zen_data_serializers.hpp"
