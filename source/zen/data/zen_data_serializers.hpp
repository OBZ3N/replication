#pragma once

#include "zen/core/zen_core_endian.h"
#include "zen/core/zen_core_vector.h"
#include "zen/data/zen_data_serializers.h"

namespace zen
{
    namespace serializers
    {
        template<typename TYPE>
        bool serialize_raw(const TYPE& value, bitstream::Writer& out)
        {
            TYPE n_value = core::convert_to_little_endian(value);

            return out.write(&n_value, sizeof(TYPE) << 3);
        }

        template<typename TYPE>
        bool deserialize_raw(TYPE& value, bitstream::Reader& in)
        {
            TYPE n_value;

            if (!in.read(&n_value, sizeof(TYPE) << 3))
                return false;

            value = core::convert_from_little_endian(n_value);

            return in.ok();
        }

        template<typename TYPE>
        bool serialize_float_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Writer& out)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((num_bits > (TYPE)0), "num_bits(", num_bits, ") <= 0");
            ZEN_ASSERT((num_bits < 64), "num_bits(", num_bits, ") >= 64");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            constexpr uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            uint64_t n_value;

            if (value <= value_min)
            {
                n_value = 0;
            }
            else
            {
                TYPE ratio = (value - value_min) / (value_max - value_min);

                n_value = uint64_t(ratio * TYPE(n_value_max));

                if (n_value >= n_value_max)
                {
                    n_value = n_value_max;
                }
            }

            n_value = core::convert_to_little_endian(n_value);

            return out.write(&n_value, get_num_bits());
        }

        template<typename TYPE>
        bool deserialize_float_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, size_t num_bits, bitstream::Reader& in)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((num_bits > (TYPE)0), "num_bits(", num_bits, ") <= 0");
            ZEN_ASSERT((num_bits < 64), "num_bits(", num_bits, ") >= 64");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            constexpr uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            uint64_t n_value = 0;

            if (!in.read(&n_value, num_bits))
                return false;

            n_value = core::convert_from_little_endian(n_value);

            value = value_min + (TYPE(n_value) / TYPE(n_value_max));

            return in.ok();
        }

        template<typename TYPE>
        bool serialize_integer_ranged(const TYPE& value, const TYPE& value_min, const TYPE& value_max, bitstream::Writer& out)
        {
            ZEN_ASSERT((value_max >= value_min), "max(", value_max, ") < min(", value_min, ")");
            ZEN_ASSERT((value >= value_min), "value(", value, ") < min(", value_min, ")");
            ZEN_ASSERT((value <= value_max), "value(", value, ") > max(", value_max, ")");

            constexpr size_t num_bits = Log2<value_max - value_min + 1>::get();

            constexpr uint64_t n_value_max = (uint64_t(1) << uint64_t(num_bits)) - uint64_t(1);

            TYPE n_value;

            if (value <= value_min)
            {
                n_value = core::convert_to_little_endian(0);
            }
            else if (value >= value_max)
            {
                n_value = core::convert_to_little_endian(value - value_min);

                if (n_value > n_value_max)
                {
                    n_value = n_value_max;
                }
            }

            return out.write(&n_value, num_bits);
        }

        template<typename TYPE>
        bool deserialize_integer_ranged(TYPE& value, const TYPE& value_min, const TYPE& value_max, bitstream::Reader& in)
        {
            constexpr size_t num_bits = Log2<value_max - value_min + 1>::get();

            TYPE n_value = 0;
            if (!in.read(&n_value, num_bits))
                return false;

            value = value_min + core::convert_from_little_endian(n_value);
            
            return in.ok();
        }
    
        template<typename TYPE>
        bool Diff<TYPE>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return (value != reference) && out.ok();
        }

        template<typename TYPE>
        bool Diff<TYPE>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return true;
        }

        template<typename TYPE, TYPE value_default>
        bool Default<TYPE, value_default>::serialize_value(const TYPE& value, bitstream::Writer& out) const
        {
            bool is_default_value = (value == value_default);

            serialize_boolean(is_default_value, out);

            return (!is_default_value) && out.ok();
        }

        template<typename TYPE, TYPE value_default>
        bool Default<TYPE, value_default>::deserialize_value(TYPE& value, bitstream::Reader& in) const
        {
            bool is_default_value;

            deserialize_boolean(is_default_value, in);

            if (is_default_value && in.ok())
            {
                value = value_default;
                return false;
            }
            else
            {
                return in.ok();
            }
        }

        template<typename TYPE, TYPE value_default>
        bool Default<TYPE, value_default>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        template<typename TYPE, TYPE value_default>
        bool Default<TYPE, value_default>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        template<typename TYPE>
        bool Integer<TYPE>::serialize_value(const TYPE& value, bitstream::Writer& out) const
        {
            return serialize_raw(value, out);
        }

        template<typename TYPE>
        bool Integer<TYPE>::deserialize_value(TYPE& value, bitstream::Reader& in) const
        {
            return deserialize_raw(value, in);
        }

        template<typename TYPE>
        bool Integer<TYPE>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        template<typename TYPE>
        bool Integer<TYPE>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        template<typename TYPE>
        bool Float<TYPE>::serialize_value(const TYPE& value, bitstream::Writer& out) const
        {
            return serialize_raw(value, out);
        }

        template<typename TYPE>
        bool Float<TYPE>::deserialize_value(TYPE& value, bitstream::Reader& in) const
        {
            return deserialize_raw(value, in);
        }

        template<typename TYPE>
        bool Float<TYPE>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        template<typename TYPE>
        bool Float<TYPE>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool IntegerRanged<TYPE, value_min, value_max>::serialize_value(const TYPE& value, bitstream::Writer& out) const
        {
            return serialize_integer_ranged(value, value_min, value_max, out);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool IntegerRanged<TYPE, value_min, value_max>::deserialize_value(TYPE& value, bitstream::Reader& in) const
        {
            return deserialize_integer_ranged(value, value_min, value_max, in);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool IntegerRanged<TYPE, value_min, value_max>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max>
        bool IntegerRanged<TYPE, value_min, value_max>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits>
        bool FloatRanged<TYPE, value_min, value_max, num_bits>::serialize_value(const TYPE& value, bitstream::Writer& out) const
        {
            return serialize_float_ranged(value, value_min, value_max, num_bits, out);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits>
        bool FloatRanged<TYPE, value_min, value_max, num_bits>::deserialize_value(TYPE& value, bitstream::Reader& in) const
        {
            return deserialize_float_ranged(value, value_min, value_max, num_bits, in);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits>
        bool FloatRanged<TYPE, value_min, value_max, num_bits>::serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        template<typename TYPE, TYPE value_min, TYPE value_max, size_t num_bits>
        bool FloatRanged<TYPE, value_min, value_max, num_bits>::deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        template<typename TYPE, typename Serializer>
        bool serialize_value(const TYPE& value, bitstream::Writer& out, const Serializer& serializer)
        {
            return serializer.serialize_value(value, out);
        }

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool serialize_value(const TYPE& value, bitstream::Writer& out, const FirstSerializer& first_serializer, const NextSerialisers&... next_serializers)
        {
            if (!first_serializer.serialize_value(value, out))
            {
                return false;
            }
            return serialize_value(value, out, next_serializers...);
        }

        template<typename TYPE, typename Serializer>
        bool deserialize_value(TYPE& value, bitstream::Reader& in, const Serializer& serializer)
        {
            return serializer.deserialize_value(value, in);
        }

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool deserialize_value(TYPE& value, bitstream::Reader& in, const FirstSerializer& first_serializer, const NextSerialisers&... next_serializers)
        {
            if (!first_serializer.deserialize_value(value, in))
            {
                return false;
            }
            return deserialize_value(value, in, next_serializers...);
        }

        template<typename TYPE, typename Serializer>
        bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out, const Serializer& serializer)
        {
            return serializer.serialize_delta(value, reference, out);
        }

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool serialize_delta(const TYPE& value, const TYPE& reference, bitstream::Writer& out, const FirstSerializer& first_serializer, const NextSerialisers&... next_serializers)
        {
            if (!first_serializer.serialize_delta(value, reference, out))
            {
                return false;
            }
            return serialize_delta(value, reference, out, next_serializers...);
        }

        template<typename TYPE, typename Serializer>
        bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in, const Serializer& serializer)
        {
            return serializer.deserialize_delta(value, reference, in);
        }

        template<typename TYPE, typename FirstSerializer, typename... NextSerialisers>
        bool deserialize_delta(TYPE& value, const TYPE& reference, bitstream::Reader& in, const FirstSerializer& first_serializer, const NextSerialisers&... next_serializers)
        {
            if (!first_serializer.deserialize_delta(value, reference, in))
            {
                return false;
            }
            return deserialize_delta(value, reference, in, next_serializers...);
        }
    }
}
