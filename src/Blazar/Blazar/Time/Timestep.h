#pragma once

namespace Blazar {
class Timestep {
   public:
    Timestep(float time = 0.0f) : m_Time(time){};

    operator float() const { return m_Time; }

    float get_seconds() const { return m_Time; }
    float get_milliseconds() const { return m_Time * 1000.0f; }

   private:
    float m_Time;
};
}  // namespace Blazar
