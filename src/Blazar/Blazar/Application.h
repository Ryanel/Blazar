#pragma once

#include <condition_variable>
#include <mutex>
#include "Blazar/Config.h"
#include "Blazar/Core.h"
#include "Blazar/Time/Timestep.h"
#include "Blazar/Window.h"

namespace Blazar {

class ImGuiSystem;
class Window;
class RenderTexture;

namespace Scenes {
class SceneManager;
}

namespace Editor {
class Editor;
}

/// Represents a running application in Blazar
class Application {
   public:
    Application();           ///< Creates the application
    virtual ~Application();  ///< Deconstructor
    void run();              ///< Main run loop

    /// Gets the main window
    inline Window& GetWindow() { return *m_Window; }

    /// Returns this application
    inline static Application& get() { return *s_Instance; }
    Scenes::SceneManager*      scenemanager() { return m_SceneManager; }

   public:
    std::unique_ptr<Window> m_Window;  ///< This Window
    // std::shared_ptr<Simulation> m_Simulation;  ///< The Simulation State
    Timestep           m_deltaTime         = 0.016f;    ///< The time the last frame took.
    bool               m_Running           = true;      ///< Is the application running?
    bool               m_RenderImGui       = true;      ///< Should ImGUI render?
    bool               m_UseEditorWindow   = true;      ///< Renders the game in the Editor Window
    Ref<Viewport>      m_RenderViewport    = nullptr;   ///< The viewport of where the Game Renders
    Ref<RenderTexture> m_GameRenderTexture = nullptr;   ///< The Game's render texture
    std::string        m_name              = "Blazar";  ///< Name of the Application

   public:
    void virtual update() = 0;
    void virtual render() = 0;

   protected:
    static Application*   s_Instance;
    ImGuiSystem*          m_ImGui        = nullptr;
    Scenes::SceneManager* m_SceneManager = nullptr;
    Editor::Editor*       m_editor       = nullptr;  ///< Editor instance

   private:
    // Threading
    void update_thread();
    void update_thread_signal();
    void update_thread_wait();
    void update_renderbuffer();
    void render_thread();

    bool                    m_updateThreadCanWork = false;
    std::condition_variable m_updateThreadSignal;
    std::mutex              m_updateThreadLock;
};

}  // namespace Blazar
