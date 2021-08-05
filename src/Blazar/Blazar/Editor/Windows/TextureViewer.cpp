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

void TextureViewer::render() {
    ZoneScoped;

    if (loaded_resource == false) { m_tex = Texture2D::Load(m_path); }
    std::string windowName = "Texture Viewer: " + m_path;
    m_name                 = windowName;
    draw_image();
}

void TextureViewer::draw_image() {
    // Calculate texture size
    auto   contentSize = ImGui::GetContentRegionAvail();
    ImVec2 tsz(m_tex->data()->GetWidth(), m_tex->data()->GetHeight());
    float  s = std::min(contentSize.x / tsz.x, contentSize.y / tsz.y);
    ImVec2 sz(tsz.x * s, tsz.y * s);

    // Center vertically
    float pic_y = ((contentSize.y - sz.y) / 2) + ImGui::GetCursorPosY();
    ImGui::SetCursorPosY(pic_y);

    // Draw
    ImGui::Image(m_tex->data()->imgui_id(), sz, ImVec2(0, 1), ImVec2(1, 0));
}

}  // namespace Editor
}  // namespace Blazar

#endif
