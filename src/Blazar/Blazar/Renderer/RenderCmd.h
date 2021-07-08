#pragma once

#include "RendererAPI.h"

namespace Blazar {

class RenderCmd {
   public:
    inline static void Clear() { s_RendererAPI->Clear(); }
    inline static void SetClearColor(float r, float g, float b, float a) { s_RendererAPI->SetClearColor(r,g,b,a); }
    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

    inline static void SetViewport(int x, int y, int width, int height) {
        s_RendererAPI->SetViewport(x, y, width, height);
    }
};

};  // namespace Blazar
