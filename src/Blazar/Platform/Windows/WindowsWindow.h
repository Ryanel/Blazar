#include "bzpch.h"

#include <Blazar/Window.h>
#include <Blazar/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace Blazar {
class WindowsWindow : public Window {
   public:
    WindowsWindow(const WindowProperties& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override { return m_Data.Width; }
    inline unsigned int GetHeight() const override { return m_Data.Width; }

    virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;
    virtual void* GetNativeWindow() override;

   private:
    virtual void Init(const WindowProperties& props);
    virtual void Shutdown();

   private:
    GLFWwindow* m_Window;
    GraphicsContext* m_Context;
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};
};  // namespace Blazar
