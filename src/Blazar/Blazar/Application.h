#pragma once

#include <condition_variable>
#include <mutex>
#include "Blazar/Core.h"
#include "Blazar/Time/Timestep.h"
#include "Blazar/Window.h"
namespace Blazar {

class ImGuiLayer;
class Window;
class RenderTexture;
class Simulation;

namespace Editor {
class Editor;
}

/// Represents a running application in Blazar
class Application {
   public:
    Application();           ///< Creates the application
    virtual ~Application();  ///< Deconstructor
    void Run();              ///< Main run loop

    /// Gets the main window
    inline Window& GetWindow() { return *m_Window; }

    /// Returns this application
    inline static Application& Get() { return *s_Instance; }

   public:
    std::unique_ptr<Window>     m_Window;      ///< This Window
    std::shared_ptr<Simulation> m_Simulation;  ///< The Simulation State
    Editor::Editor*             m_editor;      ///< Editor instance

    Timestep           m_deltaTime       = 0.016f;  ///< The time the last frame took.
    bool               m_Running         = true;    ///< Is the application running?
    bool               m_RenderImGui     = true;    ///< Should ImGUI render?
    bool               m_UseEditorWindow = true;    ///< Renders the game in the Editor Window
    Ref<Viewport>      m_RenderViewport;            ///< The viewport of where the Game Renders
    Ref<RenderTexture> m_GameRenderTexture;         ///< The Game's render texture
    std::string        m_name;                      ///< Name of the game

   public:
    void virtual OnUpdate() = 0;
    void virtual OnRender() = 0;

   private:
    void UpdateThread();

   private:
    static Application*     s_Instance;
    ImGuiLayer*             m_ImGui;
    std::condition_variable m_updateThreadSignal;
    std::mutex              m_updateThreadLock;
    bool                    m_updateThreadCanWork = false;
};

}  // namespace Blazar
