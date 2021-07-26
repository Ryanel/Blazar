#pragma once

#include "entt/entt.hpp"

namespace Blazar {

class SimulationStatsWindow;
class Quad;
/// The simulation system of the game
class Simulation {
   public:
    void Init();                  ///< Initial setup
    void Tick(float timestep);    ///< Tick System
    void Render(float timestep);  ///< Tick System

   public:
    entt::registry world;  ///< Entity World

   private:
    int   UpdateTime(float timestep);  ///< Updates the time
    float m_simTime        = 0.0f;     ///< The current time of the simulation
    float m_simTimeAccrued = 0.0f;     ///< The accrued simulation time
    float m_tickrate       = 60.0f;    ///< The tickrate
    float m_tickDelta      = 0.0f;     ///< The time a tick takes
    long  m_ticks          = 0.0f;     ///< Number of ticks that have happened
    friend class SimulationStatsWindow;

    // Rendering
private:
    Quad* quad;
};

}  // namespace Blazar
