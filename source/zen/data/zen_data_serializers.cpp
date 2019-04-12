
#include "zen/data/zen_data_serializers.h"

namespace zen
{
    namespace serializers
    {
        bool serialize_boolean(bool value, bitstream::Writer& out)
        {
            uint8_t flag = (value) ? 1 : 0;

            return out.write(&flag, 1);
        }

        bool deserialize_boolean(bool& value, bitstream::Reader& in)
        {
            uint8_t flag = 0;
            
            if (!in.read(&flag, 1))
                return false;

            value = (flag != 0);
            
            return in.ok();
        }

        bool serialize_string(const std::string& value, bitstream::Writer& out)
        {
            size_t length = value.length();

            const char* buffer = &(value[0]);

            return out.write(buffer, (length << 3));
        }

        bool deserialize_string(std::string& value, bitstream::Reader& in)
        {
            std::stringstream n_value;

            char character;
            do
            {
                if (!in.read(&character, 8))
                    return false;

                n_value << character;
            } 
            while ( character != '\0' );

            value = n_value.str();
            
            return in.ok();
        }

        bool Boolean::serialize_value(const bool& value, bitstream::Writer& out) const
        {
            return serialize_boolean(value, out);
        }

        bool Boolean::deserialize_value(bool& value, bitstream::Reader& in) const
        {
            return deserialize_boolean(value, in);
        }

        bool Boolean::serialize_delta(const bool& value, const bool& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        bool Boolean::deserialize_delta(bool& value, const bool& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }

        bool String::serialize_value(const std::string& value, bitstream::Writer& out) const
        {
            return serialize_string(value, out);
        }

        bool String::deserialize_value(std::string& value, bitstream::Reader& in) const
        {
            return deserialize_string(value, in);
        }

        bool String::serialize_delta(const std::string& value, const std::string& reference, bitstream::Writer& out) const
        {
            return serialize_value(value, out);
        }

        bool String::deserialize_delta(std::string& value, const std::string& reference, bitstream::Reader& in) const
        {
            return deserialize_value(value, in);
        }
    }
}
