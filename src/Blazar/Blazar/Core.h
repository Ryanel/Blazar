#pragma once

#include "Blazar/Config.h"
#include "Blazar/Memory.h"
#define BIT(x) (1 << x)

#ifdef BLAZAR_PLATFORM_WINDOWS

#ifdef BLAZAR_BUILD_STATIC
#define BLAZAR_API
#endif

#ifdef BLAZAR_BUILD_SHARED
#ifdef BLAZAR_BUILD_DLL
#define BLAZAR_API __declspec(dllexport)
#else
#define BLAZAR_API __declspec(dllimport)
#endif
#endif

#define DEBUGBREAK() __debugbreak()

#else
#define BLAZAR_API
//#error Blazar only supports windows.

#define DEBUGBREAK()
#endif

#ifdef BLAZAR_ENABLE_ASSERTS
#define BLAZAR_ASSERT(x, ...)                                     \
    {                                                             \
        if (!(x)) {                                               \
            LOG_GAME_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            DEBUGBREAK();                                         \
        }                                                         \
    }
#define BLAZAR_CORE_ASSERT(x, ...)                                \
    {                                                             \
        if (!(x)) {                                               \
            LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            DEBUGBREAK();                                         \
        }                                                         \
    }

#define BLAZAR_VERIFY(x, ...)                                             \
    {                                                                     \
        if (!(x)) {                                                       \
            LOG_GAME_ERROR("Runtime Assertion Failed: {0}", __VA_ARGS__); \
            DEBUGBREAK();                                                 \
        }                                                                 \
    }
#define BLAZAR_CORE_VERIFY(x, ...)                                        \
    {                                                                     \
        if (!(x)) {                                                       \
            LOG_CORE_ERROR("Runtime Assertion Failed: {0}", __VA_ARGS__); \
            DEBUGBREAK();                                                 \
        }                                                                 \
    }
#else

#define BLAZAR_ASSERT(x, ...)
#define BLAZAR_CORE_ASSERT(x, ...)
#define BLAZAR_VERIFY(x, ...) x
#define BLAZAR_CORE_VERIFY(x, ...) x

#endif

#define BLAZAR_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
