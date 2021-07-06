//
// Basic instrumentation profiler
// Based on https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e by The Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in
//     profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile
// name.
//
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

#include <thread>

namespace Blazar {

struct ProfileResult {
    std::string Name;
    long long Start, End;
    std::thread::id ThreadID;
};

struct InstrumentationSession {
    std::string Name;
};

class Instrumentor {
    Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

   public:
    void BeginSession(const std::string& name, const std::string& filepath = "profile.json") {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void EndSession() {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result) {
        if (m_ProfileCount++ > 0) m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        // We aren't going to flush every profile, this would be too expensive
        //m_OutputStream.flush();
    }

    void WriteHeader() {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        // m_OutputStream.flush();
    }

    void WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& Get() {
        static Instrumentor instance;
        return instance;
    }

   private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
};

class InstrumentationTimer {
   public:
    InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer() {
        if (!m_Stopped) Stop();
    }

    void Stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        Instrumentor::Get().WriteProfile({m_Name, start, end, std::this_thread::get_id()});

        m_Stopped = true;
    }

   private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};

}  // namespace Blazar

#define BLAZAR_PROFILE 1

#if BLAZAR_PROFILE
#define BLAZAR_PROFILE_BEGIN_SESSION(name, filepath) ::Blazar::Instrumentor::Get().BeginSession(name, filepath)
#define BLAZAR_PROFILE_END_SESSION() ::Blazar::Instrumentor::Get().EndSession()
#define BLAZAR_PROFILE_SCOPE(name) ::Blazar::InstrumentationTimer timer##__LINE__(name);
#define BLAZAR_PROFILE_FUNCTION() BLAZAR_PROFILE_SCOPE(__FUNCSIG__)
#else
#define BLAZAR_PROFILE_BEGIN_SESSION(name, filepath)
#define BLAZAR_PROFILE_END_SESSION()
#define BLAZAR_PROFILE_FUNCTION(name)
#define BLAZAR_PROFILE_SCOPE()
#endif
