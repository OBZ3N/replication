
#pragma once

#include <cinttypes>

namespace zen
{
    namespace bitstream
    {
        class Writer
        {
        public:
            enum class Result
            {
                Ok = 0,
                OutOfMemory,
                PositionOverflow,
                WriteOverflow,
            };

            Writer();
            Writer(uint32_t* bitfields, size_t bitfield_capacity);
            virtual ~Writer();

            template<typename TYPE>  bool poke( const TYPE& value, size_t position=0);
            bool poke(const void* bits, size_t num_bits, size_t position=0);

            template<typename TYPE>  bool write( const TYPE& value);
            bool write(const void* bits, size_t num_bits);
            
            Result get_last_result() const;
            void set_last_result(Result result);
            void clear_last_result();
            bool ok() const;

            bool   set_bitcount(size_t new_bitcount);
            size_t bitcount() const;
            bool   empty() const;
            bool   overflow(size_t num_bits) const;

            size_t get_bits_left(size_t position) const;
            size_t get_bits_left() const;

            void   get_bitfields(void*& bitfields, size_t& bitcount);

        protected:
            virtual bool allocate(uint32_t*& bitfields, size_t& bitcapacity);

        private:
            uint32_t*   m_bitfields;
            size_t      m_bitcount;
            size_t      m_bitcapacity;
            Result      m_last_result;

        private:
            bool write_internal(const void* bits, size_t num_bits, size_t position);
            void write_uint32_internal(uint32_t bits, size_t num_bits, size_t position);
        };
    
        template<typename TYPE>
        bool Writer::write( const TYPE& value ) 
        { 
            return write( &value, sizeof(value) << 3 ); 
        }
    
        template<typename TYPE>
        bool Writer::poke( const TYPE& value, size_t position ) 
        { 
            return poke( &value, sizeof(value) << 3, position ); 
        }
    }
}