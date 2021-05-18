#pragma once

#include "RendererAPI.h"

namespace Blazar {

class RenderCmd {
   public:
    inline static void Clear() { s_RendererAPI->Clear(); }
    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }
};

};  // namespace Blazar
