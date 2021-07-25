#pragma once

#include <variant>

#include "glm/glm.hpp"

#include "Blazar/Color.h"
#include "Blazar/Memory.h"
#include "Blazar/Rectangle.h"
#include "Cameras/Camera.h"
#include "Primitives/RenderTexture.h"
#include "Primitives/Shader.h"
#include "Primitives/Texture.h"
#include "Primitives/VertexArray.h"

namespace Blazar {

enum class RenderCommandID {
    NONE,                   ///< None
    FRAME_SYNC,             ///< Waits for VSYNC to finish to continue processing. The frame is done.
    SET_SHADER,             ///< Sets the current shader
    BIND_VERTEX_ARRAY,      ///< Binds a vertex arrray
    DRAW_VERTEX_ARRAY,      ///< Draws a vertex array
    BIND_TEXTURE2D,         ///< Binds a texture to the current context,
    BIND_TEXTURE2D_RAW,     ///< Binds a raw texture to the current context
    CLEAR_COLOR,            ///< Sets the clear color
    PASS_START,             ///< Starts a pass
    PASS_END,               ///< Ends a pass
    PASS_SET_CAMERA,        ///< Sets the camera for a pass.
    SET_VIEWPORT,           ///< Sets the viewport
    SET_RENDERTEXTURE,      ///< Sets a rendertexture
    CAMERA_SETSHADERPROPS,  ///< Sets the viewprojection from the currentCamera
    SET_MAT4,               ///< Set a mat4 on the current shader
};

/// Returns the string representation of a render command
const char* RenderCommand_GetString(RenderCommandID v);

/// A command to the Renderer
class RenderCommand {
   public:
    RenderCommand() : m_id(RenderCommandID::NONE), data(std::monostate()) {}
    RenderCommand(RenderCommandID id) : m_id(id), data(std::monostate()) {}
    RenderCommand(RenderCommandID id, Color& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Rectangle& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, std::pair<Ref<Texture2D>, int>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, std::pair<Texture2D*, int>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<RenderTexture>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<Shader>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<Camera>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, Ref<VertexArray>& data) : m_id(id), data(data) {}
    RenderCommand(RenderCommandID id, std::pair<std::string, glm::mat4>& data) : m_id(id), data(data) {}

   public:
    RenderCommandID m_id;  ///< The type of thing this RenderCommand does
    std::variant<std::monostate, std::pair<Ref<Texture2D>, int>, std::pair<Texture2D*, int>, Ref<Shader>,
                 Ref<VertexArray>, Ref<Camera>, Color, Rectangle, std::pair<std::string, glm::mat4>, Ref<RenderTexture>>
        data;
};
}  // namespace Blazar
