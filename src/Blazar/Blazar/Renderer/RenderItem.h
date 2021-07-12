#pragma once

#include "Cameras/Camera.h"
#include "Primitives/RenderTexture.h"
#include "Primitives/Shader.h"
#include "Primitives/Texture.h"
#include "Primitives/VertexArray.h"

namespace Blazar {

enum class RenderItemType {
    FRAME_SYNC,             // Waits for VSYNC to finish to continue processing. The frame is done.
    SET_SHADER,             // Sets the current shader
    BIND_VERTEX_ARRAY,      // Binds a vertex arrray
    DRAW_VERTEX_ARRAY,      // Draws a vertex array
    BIND_TEXTURE2D,         // Binds a texture to the current context
    CLEAR,                  // Clears the entire buffer
    CLEAR_COLOR,        // Sets the clear color
    PASS_START,             // Starts a pass
    PASS_END,               // Ends a pass
    PASS_SET_CAMERA,        // Sets the camera for a pass.
    SET_VIEWPORT,           // Sets the viewport
    SET_RENDERTEXTURE,      // Sets a rendertexture
    CAMERA_SETSHADERPROPS,  // Sets the viewprojection from the currentCamera
    SET_MAT4,               // Set a mat4 on the current shader
};

inline const char* RenderItemString(RenderItemType v) {
    switch (v) {
        case Blazar::RenderItemType::FRAME_SYNC:
            return "Frame Sync";
        case Blazar::RenderItemType::SET_SHADER:
            return "Set Shader";
        case Blazar::RenderItemType::BIND_VERTEX_ARRAY:
            return "Bind VAO";
        case Blazar::RenderItemType::DRAW_VERTEX_ARRAY:
            return "Render VAO";
        case Blazar::RenderItemType::BIND_TEXTURE2D:
            return "Shader <- Texture2D";
        case Blazar::RenderItemType::CLEAR_COLOR:
            return "Render Clear";
        case Blazar::RenderItemType::PASS_START:
            return "Start Pass";
        case Blazar::RenderItemType::PASS_END:
            return "End Pass";
        case Blazar::RenderItemType::PASS_SET_CAMERA:
            return "Pass Camera";
        case Blazar::RenderItemType::SET_VIEWPORT:
            return "Viewport";
        case Blazar::RenderItemType::SET_RENDERTEXTURE:
            return "Render Texture";
        case Blazar::RenderItemType::SET_MAT4:
            return "Shader <- Mat4";
        case Blazar::RenderItemType::CAMERA_SETSHADERPROPS:
            return "Shader <- CameraProps";
        default:
            return "Unknown";
    }
}

class RenderItem {
   public:
    RenderItemType type;

    RenderItem(RenderItemType t) : type(t) {}
    virtual ~RenderItem() {}
};

class RenderItem_ClearColor : public RenderItem {
   public:
    RenderItem_ClearColor(float r, float g, float b, float a)
        : RenderItem(RenderItemType::CLEAR_COLOR), r(r), g(g), b(b), a(a) {}

    float r, g, b, a;
};

class RenderItem_SetViewport : public RenderItem {
   public:
    RenderItem_SetViewport(int x, int y, int width, int height)
        : RenderItem(RenderItemType::SET_VIEWPORT), x(x), y(y), w(width), h(height) {}

    int x, y, w, h;
};

class RenderItem_SetShader : public RenderItem {
   public:
    RenderItem_SetShader(Ref<Shader> shader) : RenderItem(RenderItemType::SET_SHADER), shader(shader) {}

    Ref<Shader> shader;
};

class RenderItem_DrawIndexed : public RenderItem {
   public:
    RenderItem_DrawIndexed(Ref<VertexArray> vao) : RenderItem(RenderItemType::DRAW_VERTEX_ARRAY), vao(vao) {}

    Ref<VertexArray> vao;
};

class RenderItem_BindTexture : public RenderItem {
   public:
    RenderItem_BindTexture(Texture2D* texture, int slot = 0)
        : RenderItem(RenderItemType::BIND_TEXTURE2D), texture(texture), slot(slot) {}

    Texture2D* texture;
    int slot;
};

class RenderItem_PassSetCamera : public RenderItem {
   public:
    RenderItem_PassSetCamera(Camera* cam) : RenderItem(RenderItemType::PASS_SET_CAMERA), cam(cam) {}

    Camera* cam;
};

class RenderItem_SetRenderTexture : public RenderItem {
   public:
    RenderItem_SetRenderTexture(Ref<RenderTexture> tex) : RenderItem(RenderItemType::SET_RENDERTEXTURE), tex(tex) {}

    Ref<RenderTexture> tex;
};

class RenderItem_SetShaderMat4 : public RenderItem {
   public:
    RenderItem_SetShaderMat4(std::string str, glm::mat4& mat)
        : RenderItem(RenderItemType::SET_MAT4), str(str), mat(mat) {}

    std::string str;
    glm::mat4 mat;
};

}  // namespace Blazar
