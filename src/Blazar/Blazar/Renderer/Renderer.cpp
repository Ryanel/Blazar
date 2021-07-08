#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Renderer.h"

std::string string_ViewProjectionUniform = "u_ViewProjection";
std::string string_Transform = "u_Transform";

namespace Blazar {

RendererStats renderer_stats;
RendererAPI* s_RendererAPI;

PassData* Renderer::m_PassData = nullptr;

void Renderer::Init(RendererAPI::API toCreate) {
    BLAZAR_CORE_ASSERT(s_RendererAPI == nullptr, "Attempting to add another RendererAPI, not allowed!");
    m_PassData = new PassData();
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
void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,
                      const glm::mat4& transform) {
    ZoneScoped;
    shader->Bind();

    auto oglShader = std::dynamic_pointer_cast<OpenGLShader>(shader);

    if (oglShader) {
        oglShader->SetMat4(string_ViewProjectionUniform, m_PassData->MatViewProjection); 
        oglShader->SetMat4(string_Transform, transform);
    } else {
        BLAZAR_CORE_ASSERT(false, "Shader is Null!");
    }


    vertexArray->Bind();
    RenderCmd::DrawIndexed(vertexArray);
    renderer_stats.drawCalls++;
}

void Renderer::NewFrame() {
    renderer_stats.passesThisFrame = 0;
    renderer_stats.drawCalls = 0;
}

void Renderer::BeginPass(Camera& cam) {
    ZoneScoped;
    cam.BeginPass();
    renderer_stats.passesThisFrame++; 
    m_PassData->MatViewProjection = cam.GetViewProjection();
}
void Renderer::EndPass() {}

}  // namespace Blazar
