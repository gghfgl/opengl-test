#pragma once

#include <stdarg.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

#define ASSERT(x) if (!(x)) __debugbreak();

namespace Log
{
    void debug(const char* fmt, ...)
    {
        std::string info = "[*DEBUG] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void info(const char* fmt, ...)
    {
        std::string info = "[INFO] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void warn(const char* fmt, ...)
    {
        std::string info = "[WARN] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void error(const char* fmt, ...)
    {
        std::string info = "[ERROR] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }
}