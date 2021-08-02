#pragma once

#include <chrono>

namespace Blazar {

/// Simple timer with RAII
class Timer {
   public:
    Timer() { m_Start = std::chrono::high_resolution_clock::now(); }

    float elapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_Start);
        return (float)(dur.count() * 0.001 * 0.001 * 0.001);
    }
    float elapsed_milli() const { return elapsed() * 1000.0f; }
    void  reset() { m_Start = std::chrono::high_resolution_clock::now(); }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
}  // namespace Blazar
