#pragma once

#include "entt/entt.hpp"

namespace Blazar {

/// The simulation system of the game
class Simulation {
   public:
    void Tick(float timestep);  ///< Tick System

   public:
    entt::registry world;  ///< Entity World

   private:
    int   UpdateTime(float timestep);  ///< Updates the time
    float m_simTime        = 0.0f;     ///< The current time of the simulation
    float m_simTimeAccrued = 0.0f;     ///< The accrued simulation time
    float m_tickrate       = 60.0f;    ///< The tickrate
    float m_tickDelta      = 0.0f;     ///< The time a tick takes
};

}  // namespace Blazar
