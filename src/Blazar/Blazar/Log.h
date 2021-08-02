#pragma once

#include <memory>
#include <mutex>

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "Blazar/Core.h"

namespace Blazar {

struct log_entry {
    std::string              msg;
    std::string              time_fmt;
    spdlog::details::log_msg details;
};

class Log {
   public:
    /// Initializes the log
    static void init();
    /// Returns Blazar's logger
    inline static std::shared_ptr<spdlog::logger>& get_core() { return s_CoreLogger; }
    /// Returns the Game's logger
    inline static std::shared_ptr<spdlog::logger>& get_game() { return s_ClientLogger; }
    /// Pushes a log_entry to the list
    static void PushLog(log_entry& entry);
    /// A list of all log entries
    static std::vector<log_entry> s_LogEntries;
    /// The maximum amount of log entries
    static int s_MaxLogEntries;

   private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

template<typename Mutex> class memory_logger : public spdlog::sinks::base_sink<Mutex> {
   protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
        // msg.raw contains pre formatted log

        // If needed (very likely but not mandatory), the sink formats the message before sending it to its final
        // destination:
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        log_entry lEntry;
        lEntry.msg      = fmt::to_string(formatted);
        lEntry.details  = msg;
        lEntry.time_fmt = fmt::format("{0:%I:%M:%S %p}", msg.time);
        Log::PushLog(lEntry);
    }

    void flush_() override {}
};

using memory_logger_mt = memory_logger<std::mutex>;
using memory_logger_st = memory_logger<spdlog::details::null_mutex>;

};  // namespace Blazar

#define LOG_CORE_TRACE(...) ::Blazar::Log::get_core()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::Blazar::Log::get_core()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::Blazar::Log::get_core()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::Blazar::Log::get_core()->error(__VA_ARGS__)
#define LOG_CORE_FATAL(...) ::Blazar::Log::get_core()->fatal(__VA_ARGS__)

#define LOG_GAME_TRACE(...) ::Blazar::Log::get_game()->trace(__VA_ARGS__)
#define LOG_GAME_INFO(...) ::Blazar::Log::get_game()->info(__VA_ARGS__)
#define LOG_GAME_WARN(...) ::Blazar::Log::get_game()->warn(__VA_ARGS__)
#define LOG_GAME_ERROR(...) ::Blazar::Log::get_game()->error(__VA_ARGS__)
#define LOG_GAME_FATAL(...) ::Blazar::Log::get_game()->fatal(__VA_ARGS__)
