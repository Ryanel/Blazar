#pragma once

#include <condition_variable>
#include <mutex>
#include "Blazar/Core.h"
#include "Blazar/Layer/LayerStack.h"
#include "Blazar/Time/Timestep.h"
#include "Blazar/Window.h"

namespace Blazar {

class ImGuiLayer;
class Window;
class RenderTexture;
class Simulation;

/// Represents a running application in Blazar
class Application {
   public:
    Application();                   ///< Creates the application
    virtual ~Application();          ///< Deconstructor
    void Run();                      ///< Main run loop
    void PushLayer(Layer* layer);    ///< Pushes a layer to the application
    void PushOverlay(Layer* layer);  ///< Pushes an overlay layer to the application
    void UpdateThread();

    /// Gets the main window
    inline Window& GetWindow() { return *m_Window; }

    /// Returns this application
    inline static Application& Get() { return *s_Instance; }

   public:
    LayerStack m_LayerStack;                   ///< This layer stack
    std::unique_ptr<Window> m_Window;          ///< This Window
    std::shared_ptr<Simulation> m_Simulation;  ///< The Simulation State
    Timestep m_deltaTime = 0.016f;             ///< The Delta Time
    bool m_Running = true;                     ///< Is the application running?
    bool m_RenderImGui = true;                 ///< Should ImGUI render?
    bool m_UseEditorWindow = true;             ///< Renders the game in the Editor Window
    Ref<Viewport> m_RenderViewport;            ///< The viewport of where the Game Renders
    Ref<RenderTexture> m_GameRenderTexture;    ///< The Game's render texture

   private:
    static Application* s_Instance;
    ImGuiLayer* m_ImGui;
    std::condition_variable m_updateThreadSignal;
    std::mutex m_updateThreadLock;
    bool m_updateThreadCanWork = false;
};

}  // namespace Blazar
