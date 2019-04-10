#pragma once

#include <cinttypes>

#define ZEN_IS_BIG_ENDIAN (false)//(*((uint16_t*)"\0\xff") < 0x100)

namespace zen
{
    namespace core
    {
        template<typename TYPE>
        inline TYPE endian_swap(const TYPE& v)
        {
            TYPE n;
            switch (sizeof(TYPE))
            {
            case 8: //64-bit
                ((uint8_t*)&n)[0] = ((uint8_t*)&v)[7];
                ((uint8_t*)&n)[1] = ((uint8_t*)&v)[6];
                ((uint8_t*)&n)[2] = ((uint8_t*)&v)[5];
                ((uint8_t*)&n)[3] = ((uint8_t*)&v)[4];
                ((uint8_t*)&n)[4] = ((uint8_t*)&v)[3];
                ((uint8_t*)&n)[5] = ((uint8_t*)&v)[2];
                ((uint8_t*)&n)[6] = ((uint8_t*)&v)[1];
                ((uint8_t*)&n)[7] = ((uint8_t*)&v)[0];
                break;
            case 4: //32-bit
                ((uint8_t*)&n)[0] = ((uint8_t*)&v)[3];
                ((uint8_t*)&n)[1] = ((uint8_t*)&v)[2];
                ((uint8_t*)&n)[2] = ((uint8_t*)&v)[1];
                ((uint8_t*)&n)[3] = ((uint8_t*)&v)[0];
                break;
            case 2: //16-bit
                ((uint8_t*)&n)[0] = ((uint8_t*)&v)[1];
                ((uint8_t*)&n)[1] = ((uint8_t*)&v)[0];
                break;
            default:
                ZEN_ASSERT(false, "invalid type of size ", sizeof(TYPE), " for endian swap.");
                break;
            }
            return n;
        }

        template<typename TYPE>
        inline TYPE convert_to_little_endian(TYPE value)
        {
        #if ZEN_IS_BIG_ENDIAN
            return endian_swap(value);
        #else
            return value;
        #endif
        }


        template<typename TYPE>
        inline TYPE convert_from_little_endian(TYPE value)
        {
        #if ZEN_IS_BIG_ENDIAN
            return endian_swap(value);
        #else
            return value;
        #endif
    }

        template<typename TYPE>
        inline TYPE hton(TYPE value)
        { 
        #if ZEN_IS_BIG_ENDIAN
            return value;
        #else   
            return endian_swap(value); 
        #endif
        }
        
        template<typename TYPE>
        inline TYPE ntoh(TYPE value)
        { 
        #if ZEN_IS_BIG_ENDIAN
            return value;
        #else   
            return endian_swap(value);
        #endif
        }
   }
}