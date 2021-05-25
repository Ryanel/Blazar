#pragma once

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include <mutex>

#include <memory>
#include "Blazar/Core.h"

namespace Blazar {

struct log_entry {
    std::string msg;
    std::string time_fmt;
    spdlog::details::log_msg details;
};

template <typename Mutex>
class memory_logger : public spdlog::sinks::base_sink<Mutex> {
   protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.raw contains pre formatted log

        // If needed (very likely but not mandatory), the sink formats the message before sending it to its final
        // destination:
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        log_entry lEntry;
        lEntry.msg = fmt::to_string(formatted);
        lEntry.details = msg;
        lEntry.time_fmt = fmt::format("{0:%I:%M:%S %p}", msg.time);
        Log::PushLog(lEntry);
    }

    void flush_() override {}
};

using memory_logger_mt = memory_logger<std::mutex>;
using memory_logger_st = memory_logger<spdlog::details::null_mutex>;

class Log {
   public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return s_ClientLogger; }

    static void PushLog(log_entry& entry);
    static std::vector<log_entry> s_LogEntries;
    static int s_MaxLogEntries;
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
