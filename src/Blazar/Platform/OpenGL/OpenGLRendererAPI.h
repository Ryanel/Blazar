#pragma once

#include <memory>

#include "Blazar/Renderer/VertexArray.h"
#include "Blazar/Renderer/RendererAPI.h"

namespace Blazar {

class OpenGLRendererAPI : public RendererAPI {
   public:
    virtual inline API GetAPI() override { return API::OpenGL; }
    virtual void Clear() override;
    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    virtual void SetViewport(int x, int y, int width, int height) override;
};

};  // namespace Blazar
