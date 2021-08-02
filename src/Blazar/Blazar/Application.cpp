#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/ImGui/ImGuiSystem.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"
#include "Blazar/Time/Timer.h"
#include "Blazar/Window.h"

#include "Tracy.hpp"

#include <thread>

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;

    // Create the Window
    m_Window.reset(Window::Create());
    m_Window->SetVSync(true);

    // Initialize the renderer
    Renderer::init(RendererAPI::API::OpenGL);

    // Initialize the editor
#ifdef BLAZAR_ENABLE_EDITOR
    m_ImGui = new ImGuiSystem();
    m_ImGui->init();
#else
    m_RenderImGui     = false;
    m_UseEditorWindow = false;
#endif

    // Set the Render Viewport
    m_RenderViewport.reset(new Viewport(0,0,1600,900));
    RenderTextureProperties renderProperties;
    renderProperties.width  = (int)m_RenderViewport->width;
    renderProperties.height = (int)m_RenderViewport->height;
    renderProperties.msaa   = 1;

    m_GameRenderTexture = RenderTexture::Create(renderProperties);

    // Create the scene manager
    m_SceneManager = new Scenes::SceneManager();
}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::run() {
    ZoneScoped;

    // Start the update thread
    std::thread updateThread([&] { this->update_thread(); });

    // Run Loop
    while (m_Running) {
        ZoneScopedN("Run loop");             // Tell profiler this is the run loop
        Timer frameTimer;                    // Start timing this frame
        update_thread_signal();              // Signal the update thread to start a new frame.
        render_thread();                     // As the render thread, start rendering commands from the update thread...
        update_thread_wait();                // Wait for the update thread to finish (not letting it cross frames!)
        m_Window->OnUpdate();                // Tell the window to swap buffers and perform any updating logic.
        m_deltaTime = frameTimer.elapsed();  // Set the next frames delta-time
        FrameMark;                           // Tell profiler a frame ended
    }

    // We're shutting down. Terminate the update thread gracefully.
    m_updateThreadCanWork = false;
    m_updateThreadSignal.notify_one();
    updateThread.join();
}

void Application::render_thread() {
    ZoneScopedN("Render");
    Renderer::reset_stats();    // Resets the renderer statistics
    update_renderbuffer();      // Updates the game's internal renderbuffer
    Renderer::process_frame();  // Flushes all commands until we encounter a FRAME_SYNC.

    // Editor
#ifdef BLAZAR_ENABLE_EDITOR
    if (m_RenderImGui) {
        // ImGUI
        ZoneScopedN("ImGUI");
        m_ImGui->begin_draw();
        m_editor->render();
        m_ImGui->end_draw(m_RenderImGui);
    }
#endif
}

void Application::update_thread() {
    tracy::SetThreadName("Update Thread");

    std::unique_lock<std::mutex> lock(m_updateThreadLock);

    // This thread continues for the runtime of the application, and recieves a signal to start working.
    while (true) {
        // Wait for the signal to start working
        m_updateThreadSignal.wait(lock, [&]() { return m_updateThreadCanWork || (!m_Running); });

        // Check if we need to terminate. If true, do so.
        if (!m_Running) { return; }

        // Next, check if we are going to toggle the editor
#ifdef BLAZAR_ENABLE_EDITOR
        if (Input::key_down(BLAZAR_KEY_GRAVE_ACCENT)) { m_RenderImGui = !m_RenderImGui; }
#endif

        m_SceneManager->update(m_deltaTime);  // Execute all update systems in the scene
        m_SceneManager->render(m_deltaTime);  // Execute all render systems in the scene
        Input::new_frame();                   // Update the input system
        m_updateThreadCanWork = false;        // Report that work has finished.
    }
}

#pragma region Helpers

void Application::update_renderbuffer() {
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

void Application::update_thread_signal() {
    ZoneScoped;
    m_updateThreadCanWork = true;
    m_updateThreadSignal.notify_one();
}

void Application::update_thread_wait() {
    ZoneScoped;
    while (m_updateThreadCanWork != false) { std::this_thread::yield(); }
    m_updateThreadCanWork = true;
}

#pragma endregion

}  // namespace Blazar
