#pragma once

#include <deque>
#include "Blazar/Config.h"
#include "RendererAPI.h"

namespace Blazar {

class RendererState;
class RenderCommand;

class Renderer {
   public:
    static void Init(RendererAPI::API toCreate);
    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
    static void Submit(RenderCommand& command);
    static void Submit(RenderCommand&& command);
    static void FlushQueue();
    static void ResetStats();

    struct RendererStats {
        int passesThisFrame;
        int drawCalls;
    };

   public:
    static std::deque<RenderCommand> m_RenderQueue;

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    static std::deque<RenderCommand> m_LastRenderQueue;
#endif
    static RendererState m_CurrentState;

    static RendererStats m_stats;
};
}  // namespace Blazar
