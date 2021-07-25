#pragma once

#include <deque>
#include "Blazar/Config.h"
#include "RendererAPI.h"

namespace Blazar {

class RendererState;
class RenderCommand;

/// The main renderer class in Blazar.
/// Every frame, during the render phase, RenderCommands are submitted to the Renderer. RenderCommands are
/// Platform-Agnostic. After all commands have been submitted, the queue of commands is proceesed in FlushQueue() by the
/// Application. This is when OpenGL or DirectX calls are processed.
class Renderer {
   public:
    /// Initializes the Renderer to use a certain RendererAPI
    static void Init(RendererAPI::API toCreate);

    /// Returns the current RendererAPI
    static RendererAPI::API GetAPI() { return s_RendererAPI->GetAPI(); }

    static void Submit(RenderCommand& command);   ///< Submits a renderer command
    static void Submit(RenderCommand&& command);  ///< Submits a renderer command

    /// Flushes the Render Queue, processing commands, and calling the Graphics card
    static void FlushQueue();

    /// Resets statistics
    static void ResetStats();

    /// Renderer Statistics
    struct RendererStats {
        int passesThisFrame;  ///< Number of Render Passes this frame
        int drawCalls;        ///< Number of Draw Commands this frame
    };

   public:
    /// The Render Queue
    static std::deque<RenderCommand> m_RenderQueue;

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    static std::deque<RenderCommand> m_LastRenderQueue;
#endif
    static RendererState m_CurrentState;

    static RendererStats m_stats;
};
}  // namespace Blazar
