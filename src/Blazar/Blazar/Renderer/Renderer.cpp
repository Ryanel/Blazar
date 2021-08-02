#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"

#include "Blazar/Color.h"
#include "Blazar/Memory.h"
#include "Blazar/Rectangle.h"
#include "Cameras/Camera.h"
#include "Primitives/RenderTexture.h"
#include "Primitives/Shader.h"
#include "Primitives/Texture.h"
#include "Primitives/VertexArray.h"
#include "RenderCommand.h"

namespace Blazar {

RendererAPI* s_RendererAPI;

/// The current state of the renderer.
class RendererState {
   public:
    Ref<Camera>        m_Camera;         ///< The bound Camera
    Ref<Shader>        m_Shader;         ///< The bound Shader
    Texture2D*         m_Texture;        ///< The bound Texture2D
    Ref<RenderTexture> m_RenderTexture;  ///< The bound RenderTexture
};

// Renderer
Renderer::RendererStats   Renderer::m_stats;
std::deque<RenderCommand> Renderer::m_RenderQueue;
RendererState             Renderer::m_CurrentState;
Ref<Shader>               Renderer::m_fullscreenShader;
std::mutex                Renderer::m_QueueLock;

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
std::deque<RenderCommand> Renderer::m_LastRenderQueue;
#endif

void Renderer::init(RendererAPI::API toCreate) {
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

    // Now, initialize the fullscreen shader...
    m_fullscreenShader = Shader::Load("/Data/Shaders/ScreenTexture");
    m_fullscreenShader->SetName("ScreenTexture");
    m_fullscreenShader->Bind();
    std::dynamic_pointer_cast<Blazar::OpenGLShader>(m_fullscreenShader)->SetInt("u_Texture", 0);
}

void Renderer::submit(RenderCommand& command) {
    ZoneScoped;
    std::lock_guard guard(m_QueueLock);
    m_RenderQueue.emplace_back(command);
}
void Renderer::submit(RenderCommand&& command) {
    ZoneScoped;
    std::lock_guard guard(m_QueueLock);
    m_RenderQueue.emplace_back(command);
}

void Renderer::submit(std::vector<RenderCommand>& commands) {
    ZoneScoped;
    std::lock_guard guard(m_QueueLock);
    m_RenderQueue.insert(m_RenderQueue.end(), commands.begin(), commands.end());
}

void Renderer::reset_stats() {
    m_stats.passesThisFrame = 0;
    m_stats.drawCalls       = 0;
}

void Renderer::process_frame() {
    ZoneScoped;
    bool endProcessing = false;

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    m_LastRenderQueue.clear();
#endif

    while (!endProcessing) {
        while (true) {
            bool hasItem = false;
            {
                std::lock_guard g(m_QueueLock);
                hasItem = m_RenderQueue.size() != 0;
            }

            if (hasItem) {
                break;
            } else {
                // Try again. Yield for a little bit to allow the OS to schedule the locking thread
                std::this_thread::yield();
            }
        }

        // Fetch the item. Copy it so we can release the lock. This allows more submissions, even though it takes more
        // time. TODO: Prove this.
        RenderCommand item;
        {
            std::lock_guard guard(m_QueueLock);
            item = m_RenderQueue.front();
        }

        ZoneScopedN("Process Render Item");
        switch (item.m_id) {
            case RenderCommandID::SET_SHADER: {
                Ref<Shader> shader      = std::get<Ref<Shader>>(item.data);
                m_CurrentState.m_Shader = shader;
                if (m_CurrentState.m_Shader != nullptr) { shader->Bind(); }
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

                if (m_CurrentState.m_RenderTexture != nullptr) { m_CurrentState.m_RenderTexture->Unbind(); }
                m_CurrentState.m_RenderTexture = rt;

                if (rt != nullptr) { m_CurrentState.m_RenderTexture->Bind(); }
            } break;

            case RenderCommandID::PASS_SET_CAMERA: {
                Ref<Camera> cam         = std::get<Ref<Camera>>(item.data);
                m_CurrentState.m_Camera = cam;
                if (m_CurrentState.m_Camera != nullptr) { m_CurrentState.m_Camera->BeginPass(); }
            } break;

            case RenderCommandID::CAMERA_SETSHADERPROPS: {
                BLAZAR_ASSERT(m_CurrentState.m_Camera != nullptr, "No camera bound");
                BLAZAR_ASSERT(m_CurrentState.m_Shader != nullptr, "No shader bound");
                m_CurrentState.m_Shader->SetMat4("u_ViewProjection", m_CurrentState.m_Camera->GetViewProjection());
            } break;

            case RenderCommandID::SET_MAT4: {
                BLAZAR_ASSERT(m_CurrentState.m_Shader != nullptr, "No shader bound");
                auto props = std::get<std::pair<std::string, glm::mat4>>(item.data);
                m_CurrentState.m_Shader->SetMat4(props.first, props.second);
            } break;

            case RenderCommandID::DRAW_VERTEX_ARRAY: {
                BLAZAR_ASSERT(m_CurrentState.m_Shader != nullptr, "No shader bound");
                auto vao = std::get<Ref<VertexArray>>(item.data);
                s_RendererAPI->DrawIndexed(vao);
            } break;

            case RenderCommandID::BIND_TEXTURE2D: {
                BLAZAR_ASSERT(m_CurrentState.m_Shader != nullptr, "No shader bound");
                auto tex                 = std::get<std::pair<Ref<Texture2D>, int>>(item.data);
                m_CurrentState.m_Texture = tex.first.get();
                if (m_CurrentState.m_Texture != nullptr) { m_CurrentState.m_Texture->Bind(); }
            } break;

            case RenderCommandID::BIND_TEXTURE2D_RAW: {
                BLAZAR_ASSERT(m_CurrentState.m_Shader != nullptr, "No shader bound");
                auto tex                 = std::get<std::pair<Texture2D*, int>>(item.data);
                m_CurrentState.m_Texture = tex.first;
                if (m_CurrentState.m_Texture != nullptr) { m_CurrentState.m_Texture->Bind(); }
            } break;

            case RenderCommandID::PASS_START:
                m_stats.passesThisFrame++;
                break;
            case RenderCommandID::PASS_END:
                break;
            case RenderCommandID::FRAME_SYNC:
                endProcessing = true;
                break;
            default:
                LOG_CORE_WARN("Renderer::process_frame encountered unhandled item {}", RenderCommand_GetString(item.m_id));
                break;
        }

        {
            std::lock_guard guard(m_QueueLock);
#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
            m_LastRenderQueue.emplace_back(item);
#endif
            m_RenderQueue.pop_front();
        }

        std::this_thread::yield();  // Allow other threads to submit render queue items
    }
}

}  // namespace Blazar
