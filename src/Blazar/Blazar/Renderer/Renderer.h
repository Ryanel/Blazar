#pragma once

#include <deque>
#include "Blazar/Core.h"
#include "RenderItem.h"
#include "RendererAPI.h"

namespace Blazar {

struct RendererStats {
    int passesThisFrame;
    int drawCalls;
};

extern RendererStats renderer_stats;

class Renderer {
   public:
    static void Init(RendererAPI::API toCreate);
    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }
    static void Submit(RenderCommand& command);
    static void FlushQueue();

   public:
    static std::deque<RenderCommand> m_RenderQueue;
};
}  // namespace Blazar
