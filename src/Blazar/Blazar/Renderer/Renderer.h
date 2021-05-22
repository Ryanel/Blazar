#pragma once

#include "RendererAPI.h"
#include "VertexArray.h"

#include "Camera.h"
#include "Shader.h"

namespace Blazar {

struct RendererStats {
    int passesThisFrame;
    int drawCalls;
};

struct PassData {
    glm::mat4 MatViewProjection;
};

class Renderer {
   public:
    static void NewFrame();
    static void BeginPass(Camera& cam);
    static void EndPass();

    static void Init(RendererAPI::API toCreate);
    static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,
                       const glm::mat4& transform = glm::mat4(1.0f));

    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

   private:
    static PassData* m_PassData;

};
extern RendererStats renderer_stats;

};  // namespace Blazar
