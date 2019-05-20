#pragma once

#include <Windows.h>
#include <cinttypes>
#include <utility>
#include "zen/debug/zen_debug_output.h"

//#define ZEN_ASSERT(test, ...) { if( !(test) ) { zen::debug::log("ZEN_ASSERT(", #test, ", ", __FILE__, "@", __LINE__,") : ", __VA_ARGS__); zen::debug::out.flush(); __debugbreak(); } }
#define ZEN_ASSERT(test, ...) zen::debug::log_assert(test, #test, __FILE__, __LINE__, __VA_ARGS__);
#define ZEN_ASSERT_RETURN(test, ret, ...) { if(!zen::debug::log_assert(test, #test, __FILE__, __LINE__, __VA_ARGS__)) return ret; }
#define ZEN_LOG(...) { zen::debug::log(__VA_ARGS__); zen::debug::out.flush(); }

namespace zen
{
    namespace debug
    {
        template<typename Head>
        void log(const Head& head) 
        {
            zen::debug::out<<head<<"\n";
        }

        template<typename Head, typename... Args>
        void log(const Head& head, const Args&... args )
        {
            zen::debug::out<<head;
            zen::debug::log(args...);
        }

        template<typename... Args>
        bool log_assert(bool condition, const char* condition_desc, const char* file, uint32_t line, const Args&... args )
        {
            if( !condition ) 
            { 
                zen::debug::log("[ZEN_ASSERT] condition : ", condition_desc);
                zen::debug::log("             location  : ", file, "(", line, ")");
                zen::debug::log("             message   : ", args...);
                zen::debug::out.flush(); 
                __debugbreak(); 
                return false;
            } 
            return true;
        }
    }
}
