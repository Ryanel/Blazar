#include "bzpch.h"

#include "RenderCommand.h"

namespace Blazar {

const char* RenderCommand_GetString(RenderCommandID v) {
    switch (v) {
        case Blazar::RenderCommandID::FRAME_SYNC:
            return "Frame Sync";
        case Blazar::RenderCommandID::SET_SHADER:
            return "Set Shader";
        case Blazar::RenderCommandID::BIND_VERTEX_ARRAY:
            return "Bind VAO";
        case Blazar::RenderCommandID::DRAW_VERTEX_ARRAY:
            return "Render VAO";
        case Blazar::RenderCommandID::BIND_TEXTURE2D:
            return "Shader <- Texture2D";
        case Blazar::RenderCommandID::BIND_TEXTURE2D_RAW:
            return "Shader <- Texture2D*";
        case Blazar::RenderCommandID::CLEAR_COLOR:
            return "Render Clear";
        case Blazar::RenderCommandID::PASS_START:
            return "Start Pass";
        case Blazar::RenderCommandID::PASS_END:
            return "End Pass";
        case Blazar::RenderCommandID::PASS_SET_CAMERA:
            return "Pass Camera";
        case Blazar::RenderCommandID::SET_VIEWPORT:
            return "Viewport";
        case Blazar::RenderCommandID::SET_RENDERTEXTURE:
            return "Render Texture";
        case Blazar::RenderCommandID::SET_MAT4:
            return "Shader <- Mat4";
        case Blazar::RenderCommandID::CAMERA_SETSHADERPROPS:
            return "Shader <- CameraProps";
        default:
            return "Unknown";
    }
}

}  // namespace Blazar
