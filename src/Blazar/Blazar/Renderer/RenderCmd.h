#pragma once

#include "RenderItem.h"
#include "Renderer.h"
#include "RendererAPI.h"

namespace Blazar {

class RenderCmd {
   public:
    inline static void Clear() { Renderer::Submit(new RenderItem(RenderItemType::CLEAR)); }
    inline static void SetClearColor(float r, float g, float b, float a) {
        Renderer::Submit(new RenderItem_SetClearColor(r, g, b, a));
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        Renderer::Submit(new RenderItem_DrawIndexed(vertexArray));
    }

    inline static void SetViewport(int x, int y, int width, int height) {
        Renderer::Submit(new RenderItem_SetViewport(x, y, width, height));
    }

    inline static void BeginPass() { Renderer::Submit(new RenderItem(RenderItemType::PASS_START)); }
    inline static void UploadCameraProps() { Renderer::Submit(new RenderItem(RenderItemType::CAMERA_SETSHADERPROPS)); }
    inline static void EndPass() { Renderer::Submit(new RenderItem(RenderItemType::PASS_END)); }
    inline static void SetShader(Ref<Shader> shader) { Renderer::Submit(new RenderItem_SetShader(shader)); }
    inline static void BindTexture(Texture2D* texture, int slot = 0) {
        Renderer::Submit(new RenderItem_BindTexture(texture, slot));
    }

    inline static void BindTexture(Ref<Texture2D> texture, int slot = 0) {
        Renderer::Submit(new RenderItem_BindTexture(texture.get(), slot));
    }
    inline static void PassSetCamera(Camera* camera) { Renderer::Submit(new RenderItem_PassSetCamera(camera)); }
    inline static void SetRenderTexture(Ref<RenderTexture> tex) {
        Renderer::Submit(new RenderItem_SetRenderTexture(tex));
    }
};

};  // namespace Blazar
