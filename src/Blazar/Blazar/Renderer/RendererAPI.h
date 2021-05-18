#pragma once

#include <memory>

#include "RendererAPI.h"

#include "VertexArray.h"

namespace Blazar {

class RendererAPI {
    // API Functions
   public:
    enum class API { None = 0, OpenGL = 1 };
    virtual inline API GetAPI() { return API::None; }

    // Methods
   public:
    // virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
};

extern RendererAPI* s_RendererAPI;

};  // namespace Blazar
