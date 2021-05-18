#pragma once

#include "VertexArray.h"
#include "RendererAPI.h"

namespace Blazar {

class Renderer {
   public:
    static void BeginPass();
    static void EndPass();

    static void Init(RendererAPI::API toCreate);
    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
};

};  // namespace Blazar
