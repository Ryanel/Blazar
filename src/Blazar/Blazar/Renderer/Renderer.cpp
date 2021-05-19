#include "bzpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "Blazar/Renderer/RenderCmd.h"

namespace Blazar {
RendererStats renderer_stats;
RendererAPI* s_RendererAPI;
void Renderer::Init(RendererAPI::API toCreate) {
    BLAZAR_CORE_ASSERT(s_RendererAPI == nullptr, "Attempting to add another RendererAPI, not allowed!");

    switch (toCreate) {
        case RendererAPI::API::OpenGL:
            s_RendererAPI = new OpenGLRendererAPI();
            break;
        case RendererAPI::API::None:
        default:
            BLAZAR_CORE_ASSERT(0, "API Does not exist");
            break;
    }
}
void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    RenderCmd::DrawIndexed(vertexArray);
    renderer_stats.drawCalls++;
}

void Renderer::NewFrame() {
    renderer_stats.passesThisFrame = 0;
    renderer_stats.drawCalls = 0;
}

void Renderer::BeginPass() { renderer_stats.passesThisFrame++; }
void Renderer::EndPass() {}

}  // namespace Blazar
