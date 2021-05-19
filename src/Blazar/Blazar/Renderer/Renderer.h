#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"

namespace Blazar {

struct RendererStats {
    int passesThisFrame;
    int drawCalls;
};

class Renderer {
   public:
    static void NewFrame();
    static void BeginPass();
    static void EndPass();

    static void Init(RendererAPI::API toCreate);
    static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
};
extern RendererStats renderer_stats;

};  // namespace Blazar
