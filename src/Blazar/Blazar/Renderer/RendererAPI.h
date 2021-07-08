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
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
};

extern RendererAPI* s_RendererAPI;

};  // namespace Blazar
