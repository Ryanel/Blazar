#pragma once

#include <Blazar/Core.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Events/Events.h>
#include <Blazar/Layer/LayerStack.h>
#include <Blazar/Window.h>
#include <Blazar/Time/Timestep.h>
#include <Blazar/Time/Timer.h>

namespace Blazar {

class BLAZAR_API Application {
   public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }
    inline static Application& Get() { return *s_Instance; }

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;

    Timestep m_deltaTime;
    Timer m_FrameTimer;

   private:
    bool OnWindowClosed(WindowCloseEvent&);
    static Application* s_Instance;
};

}  // namespace Blazar
