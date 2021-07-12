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
std::deque<RenderItem*> Renderer::m_RenderQueue;
std::deque<RenderItem*> Renderer::m_LastFrameRenderQueue;

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

void Renderer::Submit(RenderItem* item) {
    ZoneScoped;
    m_RenderQueue.push_back(item);
}

void Renderer::Flush() {
    ZoneScoped;
    while (!m_LastFrameRenderQueue.empty()) {
        RenderItem* item = m_LastFrameRenderQueue.front();

        if (item != nullptr) { delete item; }

        m_LastFrameRenderQueue.pop_front();
    }
}

void Renderer::ProcessQueue() {
    ZoneScoped;
    bool stopProcessing = false;
    bool saveQueueForAnalysis = true;

    while (!m_RenderQueue.empty()) {
        RenderItem* item = m_RenderQueue.front();

        if (item == nullptr) {
            m_RenderQueue.pop_front();
            continue;
        }

        RenderItem_SetClearColor* clearColor = nullptr;
        RenderItem_SetViewport* setViewport = nullptr;
        RenderItem_PassSetCamera* setCamera = nullptr;
        RenderItem_SetRenderTexture* setRenderTexture = nullptr;

        switch (item->type) {
            case RenderItemType::SET_CLEAR_COLOR:
                clearColor = (RenderItem_SetClearColor*)item;
                s_RendererAPI->SetClearColor(clearColor->r, clearColor->g, clearColor->b, clearColor->a);
                break;
            case RenderItemType::CLEAR:
                s_RendererAPI->Clear();
                break;
            case RenderItemType::DRAW_VERTEX_ARRAY:
                renderer_stats.drawCalls++;
                s_RendererAPI->DrawIndexed(((RenderItem_DrawIndexed*)item)->vao);
                break;

            case RenderItemType::SET_SHADER:
                m_PassData->currentShader = ((RenderItem_SetShader*)item)->shader;
                m_PassData->currentShader->Bind();
                break;

            case RenderItemType::CAMERA_SETSHADERPROPS:
                std::dynamic_pointer_cast<Blazar::OpenGLShader>(m_PassData->currentShader)
                    ->SetMat4(string_ViewProjectionUniform, m_PassData->MatViewProjection);
                break;
            case RenderItemType::BIND_TEXTURE2D:
                ((RenderItem_BindTexture*)item)->texture->Bind(((RenderItem_BindTexture*)item)->slot);
                break;

            case RenderItemType::SET_RENDERTEXTURE:
                setRenderTexture = ((RenderItem_SetRenderTexture*)item);

                if (setRenderTexture->tex != nullptr) {
                    m_PassData->currentRenderTexture = nullptr;
                    m_PassData->currentRenderTexture = setRenderTexture->tex;
                    setRenderTexture->tex->Bind();
                } else {
                    if (m_PassData->currentRenderTexture != nullptr) { m_PassData->currentRenderTexture->Unbind(); }
                    m_PassData->currentRenderTexture = nullptr;
                }
                break;

            case RenderItemType::SET_VIEWPORT:
                setViewport = (RenderItem_SetViewport*)item;
                s_RendererAPI->SetViewport(setViewport->x, setViewport->y, setViewport->w, setViewport->h);
                break;

            case RenderItemType::PASS_SET_CAMERA:
                setCamera = (RenderItem_PassSetCamera*)item;
                setCamera->cam->BeginPass();  // TODO, Fix name or usage
                m_PassData->MatViewProjection = setCamera->cam->GetViewProjection();
                break;

            case RenderItemType::PASS_START:
                renderer_stats.passesThisFrame++;
                break;

            case RenderItemType::PASS_END:
                break;
            case RenderItemType::FRAME_SYNC:
                stopProcessing = true;
                break;
            default:
                LOG_CORE_WARN("Renderer::ProcessQueue encountered unhandled item {}", (int)item->type);
                break;
        }

        clearColor = nullptr;
        setViewport = nullptr;
        setCamera = nullptr;
        setRenderTexture = nullptr;

        if (saveQueueForAnalysis) {
            m_LastFrameRenderQueue.push_back(item);
        } else {
            delete item;
        }
        m_RenderQueue.pop_front();

        if (stopProcessing) { return; }
    }
}

}  // namespace Blazar
