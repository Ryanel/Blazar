#pragma once

#include <variant>

#include "glm/glm.hpp"

#include "Blazar/Color.h"
#include "Blazar/Core.h"
#include "Blazar/Rectangle.h"
#include "Cameras/Camera.h"
#include "Primitives/RenderTexture.h"
#include "Primitives/Shader.h"
#include "Primitives/Texture.h"
#include "Primitives/VertexArray.h"
namespace Blazar {

enum RenderCommandID {
    NONE,                   // None
    FRAME_SYNC,             // Waits for VSYNC to finish to continue processing. The frame is done.
    SET_SHADER,             // Sets the current shader
    BIND_VERTEX_ARRAY,      // Binds a vertex arrray
    DRAW_VERTEX_ARRAY,      // Draws a vertex array
    BIND_TEXTURE2D,         // Binds a texture to the current context
    CLEAR,                  // Clears the entire buffer
    CLEAR_COLOR,            // Sets the clear color
    PASS_START,             // Starts a pass
    PASS_END,               // Ends a pass
    PASS_SET_CAMERA,        // Sets the camera for a pass.
    SET_VIEWPORT,           // Sets the viewport
    SET_RENDERTEXTURE,      // Sets a rendertexture
    CAMERA_SETSHADERPROPS,  // Sets the viewprojection from the currentCamera
    SET_MAT4,               // Set a mat4 on the current shader
};

inline const char* RenderCommandString(RenderCommandID v) {
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

class RenderCommand {
   public:
    RenderCommandID m_id;
    RenderCommand() : m_id(RenderCommandID::NONE), data(std::monostate()) {}
    RenderCommand(RenderCommandID id) : m_id(id), data(std::monostate()) {}

    RenderCommand(RenderCommandID id, Color& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Rectangle& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, std::pair<Ref<Texture2D>, int>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<RenderTexture>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<Shader>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<Camera>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<VertexArray>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, std::pair<std::string, glm::mat4>& data) : m_id(id), data(data) {}

   public:
    std::variant<std::monostate, std::pair<Ref<Texture2D>,int>, Ref<Shader>, Ref<VertexArray>, Ref<Camera>, Color, Rectangle,
                 std::pair<std::string, glm::mat4>, Ref<RenderTexture>>
        data;
};
}  // namespace Blazar
