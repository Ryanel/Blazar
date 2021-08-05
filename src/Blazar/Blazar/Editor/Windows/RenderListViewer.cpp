#include "bzpch.h"

#include "RenderListViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include <spdlog/fmt/fmt.h>
#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Renderer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
void RenderListViewer::ListItemRenderItem(RenderCommand& item, int index) {
    bool        isSelected = selected == index;
    std::string str        = fmt::format("{}: {}", index, RenderCommand_GetString(item.m_id));
    if (ImGui::Selectable(str.c_str(), &isSelected)) { selected = index; }
}

void RenderListViewer::render() {
    ZoneScoped;

    auto& app = Application::get();

    // Options bar
    if (ImGui::Button("Clear Selection")) { selected = -1; }
    ImGui::SameLine();
    ImGui::Text("Count: %d", Renderer::m_LastRenderQueue.size());

    // List
    if (ImGui::BeginListBox("##RenderList", ImVec2(-1, -1))) {
        int index = 0;
        for (auto& x : Renderer::m_LastRenderQueue) {
            if (x.m_id == RenderCommandID::PASS_END) { ImGui::Unindent(); }

            ListItemRenderItem(x, index++);

            if (x.m_id == RenderCommandID::PASS_START) { ImGui::Indent(); }
        }
        ImGui::EndListBox();
    }
}
}  // namespace Editor
}  // namespace Blazar

#endif
