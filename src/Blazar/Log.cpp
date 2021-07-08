#include <bzpch.h>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace Blazar {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
std::vector<log_entry> Log::s_LogEntries;
int Log::s_MaxLogEntries = 200;

void Log::Init() {
    ZoneScoped;
    spdlog::set_pattern("%^[%T] %n: %v%$");

    auto memlogger = std::make_shared<memory_logger_mt>();
    memlogger->set_pattern("%v");

    std::vector<spdlog::sink_ptr> core_sinks;
    core_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    core_sinks.push_back(memlogger);

    auto core_compied_logger = std::make_shared<spdlog::logger>("Blazar", begin(core_sinks), end(core_sinks));
    auto client_compied_logger = std::make_shared<spdlog::logger>("Game", begin(core_sinks), end(core_sinks));

    s_CoreLogger = core_compied_logger;
    s_ClientLogger = client_compied_logger;

    s_CoreLogger->set_level(spdlog::level::trace);
    s_ClientLogger->set_level(spdlog::level::trace);

    s_CoreLogger->trace("Logging enabled");
    s_ClientLogger->trace("Logging enabled");

}

void Log::PushLog(log_entry& entry) {
    if (s_LogEntries.size() >= s_MaxLogEntries) { s_LogEntries.erase(s_LogEntries.begin()); }

    s_LogEntries.push_back(entry);
}
}  // namespace Blazar
