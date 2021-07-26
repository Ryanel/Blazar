#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Simulation/Simulation.h"
#include "Blazar/Simulation/SimulationStatsWindow.h"
#include "Tracy.hpp"

namespace Blazar {

void SimulationStatsWindow::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("[Development]", "Simulation Stats", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }
    auto& sim = app.m_Simulation;

    if (ImGui::Begin("Simulation Stats", &this->show)) { 
        CImGUI_Header1("Timing"); 
        ImGui::Text("Ticks: %u ticks", sim->m_ticks);
        ImGui::Text("Tickrate: %f ticks/s", sim->m_tickrate);
        ImGui::Text("Tick delta: %f ms", sim->m_tickDelta * 1000.00f);
        ImGui::Text("Sim Time (total seconds): %f", sim->m_simTime);
        ImGui::Text("Sim Time (time till tick): %f", sim->m_simTimeAccrued);

        CImGUI_Header1("World"); 
        ImGui::Text("Number of Entities: %d", sim->world.alive());

    }
    ImGui::End();
}
}  // namespace Blazar
