#pragma once

#include <chrono>

#include "Blazar/Core.h"

namespace Blazar {


/// Simple timer with RAII
class BLAZAR_API Timer {
   public:
    Timer() { 
        m_Start = std::chrono::high_resolution_clock::now();
    }

    void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }

    float Elapsed() const {
        return (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start)
                   .count() *
               0.001 * 0.001 * 0.001);
    }

    float ElapsedMillis() const { return Elapsed() * 1000.0f; }

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};
}  // namespace Blazar
