#pragma once

#include "Blazar/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Blazar {

class OpenGLContext : public GraphicsContext {
   public:
    OpenGLContext(GLFWwindow* handle);
    OpenGLContext(const OpenGLContext&) = delete;
    virtual ~OpenGLContext() = default;
    virtual void Init() override;
    virtual void Present() override;

   private:
    GLFWwindow* m_WindowHandle = nullptr;
};

};  // namespace Blazar
