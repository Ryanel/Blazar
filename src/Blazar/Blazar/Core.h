#pragma once

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
#else
#error Blazar only supports windows.
#endif

#ifdef BLAZAR_ENABLE_ASSERTS
#define BLAZAR_ASSERT(x, ...)                                     \
    {                                                             \
        if (!(x)) {                                               \
            LOG_GAME_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                       \
        }                                                         \
    }
#define BLAZAR_CORE_ASSERT(x, ...)                                \
    {                                                             \
        if (!(x)) {                                               \
            LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                       \
        }                                                         \
    }

#define BLAZAR_VERIFY(x, ...)                                        \
    {                                                                \
        if (!(x)) {                                                  \
            LOG_GAME_ERROR("Verification Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                          \
        }                                                            \
    }
#define BLAZAR_CORE_VERIFYT(x, ...)                                  \
    {                                                                \
        if (!(x)) {                                                  \
            LOG_CORE_ERROR("Verification Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                          \
        }                                                            \
    }
#else
#define BLAZAR_ASSERT(x, ...)
#define BLAZAR_CORE_ASSERT(x, ...)

#define BLAZAR_VERIFY(x, ...) x
#define BLAZAR_CORE_VERIFY(x, ...) x
#endif

#define BLAZAR_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
