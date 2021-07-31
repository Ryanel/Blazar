#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/ImGui/ImGuiLayer.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"
#include "Blazar/Time/Timer.h"
#include "Blazar/Window.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"

#include "Tracy.hpp"

#include <thread>

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;
    m_Window.reset(Window::Create());
    m_Window->SetVSync(true);

    Renderer::Init(RendererAPI::API::OpenGL);

    m_ImGui = new ImGuiLayer();
    m_ImGui->OnAttach();
    m_RenderViewport = std::make_shared<Viewport>(0, 0, 32, 32);

    RenderTextureProperties renderProperties;
    renderProperties.width  = (int)m_RenderViewport->width;
    renderProperties.height = (int)m_RenderViewport->height;
    renderProperties.msaa   = 1;

    m_GameRenderTexture = RenderTexture::Create(renderProperties);

    m_SceneManager = new Scenes::SceneManager();
}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::UpdateThread() {
    tracy::SetThreadName("Update Thread");
    // ZoneScopedN("Update Thread");
    std::unique_lock<std::mutex> lock(m_updateThreadLock);
    while (this->m_Running) {
        m_updateThreadSignal.wait(lock, [&]() { return m_updateThreadCanWork || (!m_Running); });
        if (!m_Running) { return; }

        {
            ZoneScopedN("Update");

            if (Input::KeyDown(BLAZAR_KEY_GRAVE_ACCENT)) { m_RenderImGui = !m_RenderImGui; }
            m_SceneManager->OnUpdate(m_deltaTime);
            m_SceneManager->OnRender(m_deltaTime);

            Input::NewFrame();
            m_updateThreadCanWork = false;
        }
    }
}

void Application::Run() {
    ZoneScoped;

    // Setup the shader
    std::thread updateThread([&] { this->UpdateThread(); });
    // Run Loop
    while (m_Running) {
        ZoneScopedN("Run loop");
        Timer frameTimer;

        m_updateThreadCanWork = true;
        m_updateThreadSignal.notify_one();

        // Rendering Code
        {
            ZoneScopedN("Render");
            Renderer::ResetStats();
            {
                ZoneScopedN("Resize Render Texture");
                // Check if the renderbuffer needs to be changed.
                if (!(m_UseEditorWindow && m_RenderImGui)) {
                    m_RenderViewport->Set(0, 0, GetWindow().GetViewport()->width, GetWindow().GetViewport()->height);
                }

                RenderTextureProperties renderProperties((int)m_RenderViewport->width, (int)m_RenderViewport->height);

                if ((m_GameRenderTexture->GetWidth() != renderProperties.width) ||
                    (m_GameRenderTexture->GetHeight() != renderProperties.height)) {
                    m_GameRenderTexture = RenderTexture::Create(renderProperties);
                    m_GameRenderTexture->Unbind();
                }
            }

            // Processes the queue until we get to the FRAME_SYNC. Then ImGUI code runs
            Renderer::FlushQueue();

            if (m_RenderImGui) {
                // ImGUI
                ZoneScopedN("ImGUI");
                m_ImGui->Begin();
                m_editor->RenderWindow();
                m_ImGui->End(m_RenderImGui);
            }
        }
        // Flip Windows
        m_Window->OnUpdate();

        FrameMark;

        // Wait for the update thread to finish (not letting it cross frames!)
        while (m_updateThreadCanWork != false) { std::this_thread::yield(); }
        m_updateThreadCanWork = true;
        m_deltaTime           = frameTimer.Elapsed();
    }

    m_updateThreadCanWork = false;
    m_updateThreadSignal.notify_one();
    updateThread.join();
}

}  // namespace Blazar
