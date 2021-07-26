#pragma once

#include "RenderCommand.h"
#include "Renderer.h"
#include "RendererAPI.h"

#define RENDER_SUBMIT(x) Renderer::Submit(x)

namespace Blazar {

class RenderCmd {
   public:
    inline static RenderCommand Clear(float r, float g, float b, float a) {
        Color         data(r, g, b, a);
        RenderCommand cmd(RenderCommandID::CLEAR_COLOR, data);
        return cmd;
    }

    inline static RenderCommand DrawIndexed(Ref<VertexArray>& vertexArray) {
        RenderCommand cmd(RenderCommandID::DRAW_VERTEX_ARRAY, vertexArray);
        return cmd;
    }

    inline static RenderCommand SetViewport(int x, int y, int width, int height) {
        Rectangle     data(x, y, width, height);
        RenderCommand cmd(RenderCommandID::SET_VIEWPORT, data);
        return cmd;
    }

    inline static RenderCommand BeginPass() { return RenderCommand(RenderCommandID::PASS_START); }
    inline static RenderCommand EndPass() { return RenderCommand(RenderCommandID::PASS_END); }
    inline static RenderCommand UploadCameraProps() { return RenderCommand(RenderCommandID::CAMERA_SETSHADERPROPS); }

    inline static RenderCommand PassSetCamera(Ref<Camera> camera) {
        RenderCommand cmd(RenderCommandID::PASS_SET_CAMERA, camera);
        return cmd;
    }

    // Textures
    inline static RenderCommand BindTexture(Ref<Texture2D> texture, int slot = 0) {
        std::pair<Ref<Texture2D>, int> data(texture, slot);
        RenderCommand                  cmd(RenderCommandID::BIND_TEXTURE2D, data);
        return cmd;
    }

    inline static RenderCommand BindTexture(Texture2D* texture, int slot = 0) {
        std::pair<Texture2D*, int> data(texture, slot);
        RenderCommand              cmd(RenderCommandID::BIND_TEXTURE2D_RAW, data);
        return cmd;
    }

    inline static RenderCommand SetRenderTexture(Ref<RenderTexture> tex) {
        RenderCommand cmd(RenderCommandID::SET_RENDERTEXTURE, tex);
        return cmd;
    }

    // Shaders
    inline static RenderCommand SetShader(Ref<Shader> shader) {
        RenderCommand cmd(RenderCommandID::SET_SHADER, shader);
        return cmd;
    }

    inline static RenderCommand SetShaderMat4(std::string str, glm::mat4& mat) {
        std::pair<std::string, glm::mat4> data(str, mat);
        RenderCommand                     cmd(RenderCommandID::SET_MAT4, data);
        return cmd;
    }
};

};  // namespace Blazar
