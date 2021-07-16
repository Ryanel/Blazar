#pragma once

#include "RenderItem.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Blazar {

class RenderCmd {
   public:
    inline static void Clear(float r, float g, float b, float a) {
        Color data(r, g, b, a);
        RenderCommand cmd(RenderCommandID::CLEAR_COLOR, data);
        Renderer::Submit(cmd);
    }

    inline static void DrawIndexed(Ref<VertexArray>& vertexArray) {
        RenderCommand cmd(RenderCommandID::DRAW_VERTEX_ARRAY, vertexArray);
        Renderer::Submit(cmd);
    }

    inline static void SetViewport(int x, int y, int width, int height) {
        Rectangle data(x, y, width, height);
        RenderCommand cmd(RenderCommandID::SET_VIEWPORT, data);
        Renderer::Submit(cmd);
    }

    inline static void BeginPass() { Renderer::Submit(RenderCommand(RenderCommandID::PASS_START)); }
    inline static void EndPass() { Renderer::Submit(RenderCommand(RenderCommandID::PASS_END)); }
    inline static void UploadCameraProps() { Renderer::Submit(RenderCommand(RenderCommandID::CAMERA_SETSHADERPROPS)); }


    inline static void PassSetCamera(Ref<Camera> camera) {
        RenderCommand cmd(RenderCommandID::PASS_SET_CAMERA, camera);
        Renderer::Submit(cmd);
    }

    // Textures
    inline static void BindTexture(Ref<Texture2D> texture, int slot = 0) {
        std::pair<Ref<Texture2D>, int> data(texture, slot);
        RenderCommand cmd(RenderCommandID::BIND_TEXTURE2D, data);
        Renderer::Submit(cmd);
    }


    inline static void BindTexture(Texture2D* texture, int slot = 0) {
        std::pair<Texture2D*, int> data(texture, slot);
        RenderCommand cmd(RenderCommandID::BIND_TEXTURE2D_RAW, data);
        Renderer::Submit(cmd);
    }

    inline static void SetRenderTexture(Ref<RenderTexture> tex) {
        RenderCommand cmd(RenderCommandID::SET_RENDERTEXTURE, tex);
        Renderer::Submit(cmd);
    }

    // Shaders
    inline static void SetShader(Ref<Shader> shader) {
        RenderCommand cmd(RenderCommandID::SET_SHADER, shader);
        Renderer::Submit(cmd);
    }

    inline static void SetShaderMat4(std::string str, glm::mat4& mat) {
        std::pair<std::string, glm::mat4> data(str, mat);
        RenderCommand cmd(RenderCommandID::SET_MAT4, data);
        Renderer::Submit(cmd);
    }
};

};  // namespace Blazar
