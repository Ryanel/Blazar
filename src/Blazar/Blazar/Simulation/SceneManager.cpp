#include "bzpch.h"

#include "SceneManager.h"

#include "Blazar/Application.h"
#include "Blazar/Renderer/Primitives/Quad.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"

namespace Blazar {
namespace Scenes {

SceneManager::SceneManager() { m_quad = new Quad(); }
SceneManager::~SceneManager() {}

void SceneManager::update(Timestep& ts) {
    // Note: This happens on the update thread (not the render/main thread)

    // Forward the update to the scene, if it exists
    if (this->m_currentScene != nullptr) {
        this->m_currentScene->update(ts);
    }
}
void SceneManager::render(Timestep& ts) {
    // Note: This happens on the update thread (not the render/main thread)
    Application&               app = Application::get();
    std::vector<RenderCommand> render_cmds;

    // Step 1: Clear the main frame buffer
    // ------------------------------------------------------------------------
#ifdef BLAZAR_ENABLE_EDITOR
    render_cmds.emplace_back(RenderCmd::SetViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight()));
    render_cmds.emplace_back(RenderCmd::Clear(0.1f, 0.1f, 0.1f, 1.0f));
#endif

    // Step 2: Clear the game's main rendertexture
    // ------------------------------------------------------------------------
    render_cmds.emplace_back(RenderCmd::SetRenderTexture(app.m_GameRenderTexture));
    render_cmds.emplace_back(
        RenderCmd::SetViewport(0, 0, app.m_GameRenderTexture->GetWidth(), app.m_GameRenderTexture->GetHeight()));
    render_cmds.emplace_back(RenderCmd::Clear(0.05f, 0.1f, 0.2f, 1.0f));
    Renderer::submit(render_cmds);
    render_cmds.clear();

    // Step 3: Render the scene in this rendertexture
    // ------------------------------------------------------------------------
    if (this->m_currentScene != nullptr) {
        this->m_currentScene->render(ts);
    }

    // Step 4: If we're not in the editor, render to a quad on screen.
    // ------------------------------------------------------------------------
    render_cmds.emplace_back(RenderCmd::SetRenderTexture(nullptr));
    if (!app.m_RenderImGui || !app.m_UseEditorWindow && false) {
        ZoneScopedN("Render Texture to Quad");
        render_cmds.emplace_back(RenderCmd::SetViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight()));
        render_cmds.emplace_back(RenderCmd::SetShader(Renderer::m_fullscreenShader));
        render_cmds.emplace_back(RenderCmd::BindTexture(app.m_GameRenderTexture->m_ColorTexture));
        render_cmds.emplace_back(RenderCmd::DrawIndexed(m_quad->vao));
    }

    // Notify the Renderer that we've reached the end of the client frame.
    render_cmds.emplace_back(RenderCommand(RenderCommandID::FRAME_SYNC));
    Renderer::submit(render_cmds);
}
}  // namespace Scenes
}  // namespace Blazar
