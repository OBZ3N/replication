
#pragma once

#include <cinttypes>

namespace zen
{
    namespace bitstream
    {
        class Reader
        {
        public:
            enum class Result
            {
                Ok = 0,
                OutOfMemory,
                PositionInvalid,
                ReadOverflow,
            };

            Reader();
            Reader(const uint32_t* bitfields, size_t bitcount, size_t position=0);
            virtual ~Reader() {}

            template<typename TYPE>  bool peek( TYPE& value, size_t position );
            template<typename TYPE>  bool read( TYPE& value );

            bool read(void* bits, size_t num_bits);
            bool peek(void* bits, size_t num_bits, size_t position);

            bool set_position(size_t position);
            size_t position() const;
            size_t bitcount() const;

            Result get_last_result() const;
            void set_last_result(Result result);
            void clear_last_result();
            bool ok() const;

            bool eof() const;
            bool overflow(size_t num_bits) const;
            void get_buffer(const void*& bitfields, size_t& bitcount);

        private:
            const uint32_t* m_bitfields;
            size_t          m_bitcount;
            size_t          m_position;
            Result          m_last_result;

        private:
            bool read_internal(void* bits, size_t num_bits, size_t position);
            void read_uint32_internal(uint32_t& bits, size_t num_bits, size_t position);
        };
    
        template<typename TYPE>  
        bool Reader::peek( TYPE& value, size_t position ) 
        { 
            return peek( &value, sizeof(value) << 3, position ); 
        }

        template<typename TYPE>  
        bool Reader::read( TYPE& value ) 
        { 
            return read( &value, sizeof(value) << 3 ); 
        }
    }
}