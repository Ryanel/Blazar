#pragma once

#include <memory>

#include "Blazar/Renderer/Primitives/VertexArray.h"
#include "Blazar/Renderer/RendererAPI.h"

namespace Blazar {

class OpenGLRendererAPI : public RendererAPI {
   public:
    virtual inline API api() override { return API::OpenGL; }
    virtual void Clear() override;
    virtual void SetClearColor(float r, float g, float b, float a) override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    virtual void SetViewport(int x, int y, int width, int height) override;
};

};  // namespace Blazar
