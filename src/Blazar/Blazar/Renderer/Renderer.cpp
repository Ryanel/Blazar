#include "bzpch.h"

#include <deque>

#include "Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLRendererAPI.h"
namespace Blazar {

RendererAPI* s_RendererAPI;

// Renderer
RendererStats renderer_stats;
std::deque<RenderCommand> Renderer::m_RenderQueue;

void Renderer::Init(RendererAPI::API toCreate) {
    BLAZAR_CORE_ASSERT(s_RendererAPI == nullptr, "Attempting to add another RendererAPI, not allowed!");
    // m_PassData = new PassData();
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
void Renderer::Submit(RenderCommand& command) { m_RenderQueue.push_back(command); }
void Renderer::FlushQueue() {
    bool endProcessing = false;
    while (!m_RenderQueue.empty() && (!endProcessing)) {
        RenderCommand& item = m_RenderQueue.front();

        switch (item.m_id) {
            case RenderCommandID::SET_SHADER: {
                Ref<Shader> shader = std::get<Ref<Shader>>(item.data);
                shader->Bind();
            } break;
            case RenderCommandID::SET_VIEWPORT: {
                Rectangle& rect = std::get<Rectangle>(item.data);
                s_RendererAPI->SetViewport(rect.x, rect.y, rect.width, rect.height);
            } break;
            case RenderCommandID::CLEAR_COLOR: {
                Color& c = std::get<Color>(item.data);
                s_RendererAPI->SetClearColor(c.r, c.g, c.b, c.a);
                s_RendererAPI->Clear();
            } break;

            case RenderCommandID::SET_RENDERTEXTURE: {
                Ref<RenderTexture> rt = std::get<Ref<RenderTexture>>(item.data);

                if (rt == nullptr) {

                }
            } break;

            // case RenderCommandID::CLEAR_RENDERTEXTURE: {
            // } break;
            case RenderCommandID::PASS_START:
                break;
            case RenderCommandID::PASS_END:
                break;
            case RenderCommandID::FRAME_SYNC:
                endProcessing = true;
                break;
            default:
                LOG_CORE_WARN("Renderer::FlushQueue encountered unhandled item {}", RenderCommandString(item.m_id));
                break;
        }

        m_RenderQueue.pop_front();
    }
}
}  // namespace Blazar
