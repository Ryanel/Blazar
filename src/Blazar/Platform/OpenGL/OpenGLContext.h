#pragma once

#include "Blazar/Renderer/GraphicsContext.h"

namespace Blazar {

struct GLFWwindow;

class OpenGLContext : public GraphicsContext {
   public:
    OpenGLContext(GLFWwindow* handle);
    OpenGLContext(const OpenGLContext&) = delete;

    virtual void Init() override;
    virtual void Present() override;

   private:
    GLFWwindow* m_WindowHandle = nullptr;
};

};  // namespace Blazar
