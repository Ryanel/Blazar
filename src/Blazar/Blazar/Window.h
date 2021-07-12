#pragma once

#include "Blazar/Events/Events.h"
#include "Blazar/Renderer/Primitives/Viewport.h"

namespace Blazar {

/// A collection of Window Properties
struct WindowProperties {
    std::string Title;    ///< The Window Title of the Game
    unsigned int Width;   ///< The Height of the Window
    unsigned int Height;  ///< The Width of the Window
    unsigned int MSAA;    ///< The Current MSAA Level. 0 means MSAA is disabled

    WindowProperties(const std::string& title = "Blazar Engine", unsigned int width = 1600, unsigned int height = 900)
        : Title(title), Width(width), Height(height), MSAA(4) {}
};

/// A Desktop Window. Produces Events, and must be updated each frame
class BLAZAR_API Window {
   public:
    using EventCallbackFn = std::function<void(Events::Event&)>;

    virtual ~Window() = 0;
    virtual void OnUpdate() = 0;
    virtual unsigned int GetHeight() const = 0;
    virtual unsigned int GetWidth() const = 0;
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    float GetAspect() const { return (float)GetWidth() / (float)GetHeight(); }

    Ref<Viewport> GetViewport() const { return m_viewport; }

    static Window* Create(const WindowProperties& props = WindowProperties());
    virtual void* GetNativeWindow() = 0;

  protected:
    Ref<Viewport> m_viewport;
};

};  // namespace Blazar
