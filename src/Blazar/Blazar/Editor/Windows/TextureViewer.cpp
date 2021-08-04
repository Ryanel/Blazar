#include "bzpch.h"

#include "TextureViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Application.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void TextureViewer::render(Editor* editor) {
    ZoneScoped;

    if (loaded_resource == false) { m_tex = Texture2D::Load(m_path); }
    std::string windowName = "Texture Viewer: " + m_path;
    m_name                 = windowName;
    draw_image();

    if (!m_windowOpen) { editor->close_window(this); }
}

void TextureViewer::draw_image() {
    auto contentSize = ImGui::GetContentRegionAvail();

    float tex_width  = m_tex->data()->GetWidth();
    float tex_height = m_tex->data()->GetHeight();

    float width_scale  = contentSize.x / tex_width;
    float height_scale = contentSize.y / tex_height;

    float scalar = std::min(width_scale, height_scale);

    float resultant_width  = tex_width * scalar;
    float resultant_height = tex_height * scalar;

    ImGui::Image(m_tex->data()->imgui_id(), ImVec2(resultant_width, resultant_height), ImVec2(0, 1), ImVec2(1, 0));
}

}  // namespace Editor
}  // namespace Blazar

#endif
