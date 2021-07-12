#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"

#include "RenderListViewer.h"
#include "Tracy.hpp"

void RenderListWindowLayer::ListItemRenderItem(RenderItem* item, int index) {
    std::string str = fmt::format("{}: {}", index, RenderItemString(item->type));

    bool isSelected = selected == index;

    if (ImGui::Selectable(str.c_str(), &isSelected)) { selected = index; }
}

void RenderListWindowLayer::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "Render List", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }

    if (ImGui::Begin("Render List", &this->show)) {
        ImGui::Text("Items in RenderList: %d", Renderer::m_LastFrameRenderQueue.size());

        if (ImGui::BeginListBox("##RenderList", ImVec2(-1, -1))) {
            int i = 0;
            for (auto& x : Renderer::m_LastFrameRenderQueue) {
                if (x->type == RenderItemType::PASS_END) { ImGui::Unindent(); }
                ListItemRenderItem(x, i);
                if (x->type == RenderItemType::PASS_START) { ImGui::Indent(); }
                i++;
            }
            ImGui::EndListBox();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Render List Properties", &this->show)) {
        if (ImGui::Button("Clear Selection")) { selected = -1; }
        if (selected != -1) {
            int i = 0;
            for (auto& x : Renderer::m_LastFrameRenderQueue) {
                if (i == selected) {
                    std::string str = fmt::format("{}", RenderItemString(x->type));
                    ImGui::Text(str.c_str());

                    RenderItem_SetViewport* ri_viewport = nullptr;

                    switch (x->type) {
                        case RenderItemType::SET_VIEWPORT:
                            ri_viewport = (RenderItem_SetViewport*)x;
                            ImGui::Text("Pos: (%d, %d)", ri_viewport->x, ri_viewport->y);
                            ImGui::Text("Size: (%d, %d)", ri_viewport->w, ri_viewport->h);
                            break;
                        default:
                            break;
                    }
                }
                i++;
            }
        }
    }
    ImGui::End();
}
