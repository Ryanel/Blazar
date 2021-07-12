#pragma once

#include "Primitives/Shader.h"
#include "Primitives/VertexArray.h"
#include "RenderItem.h"
#include "RendererAPI.h"

#include "Cameras/Camera.h"

#include <queue>

namespace Blazar {

struct RendererStats {
    int passesThisFrame;
    int drawCalls;
};

struct PassData {
    glm::mat4 MatViewProjection;
    Ref<RenderTexture> currentRenderTexture = nullptr;
    Ref<Shader> currentShader = nullptr;
};

class Renderer {
   public:
    static void Init(RendererAPI::API toCreate);
    static void Submit(RenderItem* item);
    static void ProcessQueue();
    static void Flush();

    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

    static PassData* m_PassData;
    static std::deque<RenderItem*> m_RenderQueue;
    static std::deque<RenderItem*> m_LastFrameRenderQueue;
};
extern RendererStats renderer_stats;

};  // namespace Blazar
