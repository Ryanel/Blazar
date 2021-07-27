#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Renderer/Primitives/Quad.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/Simulation.h"
#include "Tracy.hpp"

namespace Blazar {
void Simulation::Init() { quad = new Quad(); }
void Simulation::Tick(float timestep) {
    ZoneScoped;
    Application& app = Application::Get();
    // NOTE: This is run on the Update Thread, not the Main Thread!

    int ticks = UpdateTime(timestep);
    m_ticks += ticks;
    // Tick each system

    if (ticks > 0) {
        //
        app.OnUpdate();
    }
}

void Simulation::Render(float timestep) {
    ZoneScoped;
    // NOTE: This is run on the Update Thread, not the Main Thread!
    // Runs RenderSystems each frame, rendering them

    Application& app = Application::Get();
    // Preparation

    // Clear the screen
    Renderer::ResetStats();
    std::vector<RenderCommand> cmds;

    cmds.emplace_back(RenderCmd::SetViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight()));
    cmds.emplace_back(RenderCmd::Clear(0.3f, 0.3f, 0.3f, 1.0f));
    cmds.emplace_back(RenderCmd::SetRenderTexture(app.m_GameRenderTexture));
    cmds.emplace_back(
        RenderCmd::SetViewport(0, 0, app.m_GameRenderTexture->GetWidth(), app.m_GameRenderTexture->GetHeight()));
    cmds.emplace_back(RenderCmd::Clear(0.05f, 0.1f, 0.2f, 1.0f));
    Renderer::SubmitList(cmds);

    cmds.clear();

    // Render Game
    app.OnRender();

    cmds.emplace_back(RenderCmd::SetRenderTexture(nullptr));

    if (!app.m_RenderImGui || !app.m_UseEditorWindow && false) {
        ZoneScopedN("Render Texture to Quad");
        cmds.emplace_back(RenderCmd::SetViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight()));
        cmds.emplace_back(RenderCmd::SetShader(Renderer::m_fullscreenShader));
        cmds.emplace_back(RenderCmd::BindTexture(app.m_GameRenderTexture->m_ColorTexture));
        cmds.emplace_back(RenderCmd::DrawIndexed(quad->vao));
    } else {
    }
    cmds.emplace_back(RenderCommand(RenderCommandID::FRAME_SYNC));
    Renderer::SubmitList(cmds);
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
