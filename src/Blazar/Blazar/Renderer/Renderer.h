#pragma once

#include <deque>
#include "Blazar/Config.h"
#include "Blazar/Core.h"
#include "RenderItem.h"
#include "RendererAPI.h"

namespace Blazar {

struct RendererStats {
    int passesThisFrame;
    int drawCalls;
};

extern RendererStats renderer_stats;

class RendererState {
   public:
    Ref<Camera> m_Camera;
    Ref<Shader> m_Shader;
    Texture2D* m_Texture;
    Ref<RenderTexture> m_RenderTexture;
};

class Renderer {
   public:
    static void Init(RendererAPI::API toCreate);
    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
    static void Submit(RenderCommand& command);
    static void Submit(RenderCommand&& command);
    static void FlushQueue();
    static void ResetStats();
    static RendererState& CurrentState();

   public:
    static std::deque<RenderCommand> m_RenderQueue;

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    static std::deque<RenderCommand> m_LastRenderQueue;
#endif
    static RendererState m_CurrentState;
};
}  // namespace Blazar
