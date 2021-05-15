#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <memory>
#include "Blazar/Core.h"

namespace Blazar {
class BLAZAR_API Log {
   public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return s_ClientLogger; }

   private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
};  // namespace Blazar

#define LOG_CORE_TRACE(...) ::Blazar::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::Blazar::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::Blazar::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::Blazar::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...) ::Blazar::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define LOG_GAME_TRACE(...) ::Blazar::Log::GetGameLogger()->trace(__VA_ARGS__)
#define LOG_GAME_INFO(...) ::Blazar::Log::GetGameLogger()->info(__VA_ARGS__)
#define LOG_GAME_WARN(...) ::Blazar::Log::GetGameLogger()->warn(__VA_ARGS__)
#define LOG_GAME_ERROR(...) ::Blazar::Log::GetGameLogger()->error(__VA_ARGS__)
#define LOG_GAME_FATAL(...) ::Blazar::Log::GetGameLogger()->fatal(__VA_ARGS__)
