#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/AppEvents.h"
#include "Blazar/Events/Events.h"
#include "Blazar/Layer/LayerStack.h"
#include "Blazar/Time/Timer.h"
#include "Blazar/Time/Timestep.h"
#include "Blazar/Window.h"
#include "Blazar/Renderer/RenderTexture.h"
#ifdef BLAZAR_IMGUI_ENABLED
#include "Blazar/ImGui/ImGuiLayer.h"
#endif

namespace Blazar {

/// Represents a running application in Blazar
class BLAZAR_API Application {
   public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Events::Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

   public:
    LayerStack m_LayerStack;
    std::unique_ptr<Window> m_Window;
    Timestep m_deltaTime = 0.016f;
    Timer m_FrameTimer;

    bool m_Running = true;
    bool m_RenderImGui = true;
    bool m_UseEditorWindow = true;
    Ref<Viewport> m_RenderViewport;

    Ref<RenderTexture> m_GameRenderTexture;

   private:
    bool OnWindowClosed(Events::WindowCloseEvent&);
    static Application* s_Instance;

#ifdef BLAZAR_IMGUI_ENABLED
    ImGuiLayer* m_ImGui;
    bool m_ImGuiShowKeyPressedLastFrame = false;

#endif
};

}  // namespace Blazar
