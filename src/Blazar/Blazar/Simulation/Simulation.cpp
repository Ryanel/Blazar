#include "bzpch.h"

#include "Blazar/Simulation/Simulation.h"

#include "Tracy.hpp"

namespace Blazar {

void Simulation::Tick(float timestep) {
    ZoneScoped;
    int ticks = UpdateTime(timestep);

    // Tick each system
}

int Simulation::UpdateTime(float timestep) {
    ZoneScoped;
    m_tickDelta = 1.0f / m_tickrate;
    m_simTimeAccrued += timestep;
    m_simTime += timestep;

    int ticksToProcess = 0;

    while (m_simTimeAccrued > m_tickDelta) {
        ticksToProcess++;
        m_simTimeAccrued -= m_tickDelta;
    }

    return ticksToProcess;
}

}  // namespace Blazar
