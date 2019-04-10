#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

namespace zen
{
    namespace core
    {
        template <class CharT, class TraitsT = std::char_traits<CharT> >
        class StringBuffer : public std::basic_stringbuf<CharT, TraitsT>
        {
        public:
            StringBuffer()
            {}

            virtual ~StringBuffer()
            {
                sync();
            }

        protected:
            int sync() override
            {
                output(str().c_str());

                str(std::basic_string<CharT>());

                return 0;
            }

            virtual void output(const CharT *text) 
            {}
        };

        template<class Buffer, class CharT, class TraitsT = std::char_traits<CharT>>
        class OStream : public std::basic_ostream<CharT, TraitsT>
        {
        public:
            OStream() 
                : std::basic_ostream<CharT, TraitsT>(new Buffer())
            {}
        
            ~OStream() 
            {
                delete rdbuf(); 
            }
        };
    }
}