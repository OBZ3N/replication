#pragma once

#include <Windows.h>
#include "zen/core/zen_core_ostream.h"

namespace zen
{
    namespace debug
    {
        template <class CharT, class TraitsT = std::char_traits<CharT> >
        class DebugStringBuffer: public core::StringBuffer<CharT, TraitsT>
        {
        public:
            DebugStringBuffer() {}
            
            virtual void output(const CharT *text)
            {}
        };

        template<>
        void DebugStringBuffer<char>::output(const char *text)
        {
            ::OutputDebugStringA(text);
            std::cout << str().c_str();
        }

        template<>
        void DebugStringBuffer<wchar_t>::output(const wchar_t *text)
        {
            ::OutputDebugStringW(text);
            std::cout << str().c_str();
        }

        template<class CharT, class TraitsT = std::char_traits<CharT> >
        class DebugOutput: public core::OStream<DebugStringBuffer<CharT, TraitsT>, CharT, TraitsT>
        {
        public:
            DebugOutput() {}
        };

        extern core::OStream<DebugStringBuffer<char>, char> out;
    }
}
