#pragma once

#include <windows.h>
#include <iostream>
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

#define KB(Value) ((Value)*1024)
#define MB(Value) (KB(Value)*1024)
#define GB(Value) (MB(Value)*1024)

#if DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
//#define ASSERT(x) if (!(x)) *(int*)0 = 0;
#else
#define ASSERT(x)
#endif

namespace Log {
    void debug(const char* fmt, ...) {
        std::string info = "[*DEBUG] ";
        info.append(fmt);

        // @improve: Seems deprecated C99
        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void info(const char* fmt, ...) {
        std::string info = "[INFO] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void warn(const char* fmt, ...) {
        std::string info = "[WARN] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }

    void error(const char* fmt, ...) {
        std::string info = "[ERROR] ";
        info.append(fmt);

        va_list args;
        va_start(args, fmt);
        vprintf(info.c_str(), args);
        va_end(args);
    }
}

struct Plateform_Memory {
    uint64 permanentStorageSize;
    void* permanentStorage;

    uint64 transientStorageSize;
    void* transientStorage;

    bool isInitialized;
};

void AllocateMemory(Plateform_Memory* memory) {
    memory->permanentStorage = VirtualAlloc(0, memory->permanentStorageSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    if (!memory->permanentStorage) {
        Log::error("MEMORY: Faild to alloc permanent storage memory with size: %d", memory->permanentStorageSize);
        exit(666);
    }

    memory->transientStorage = VirtualAlloc(0, memory->transientStorageSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    if (!memory->transientStorage) {
        Log::error("MEMORY: Faild to alloc transient storage memory with size: %d", memory->transientStorageSize);
        exit(666);
    }

    memory->isInitialized = true;
}
