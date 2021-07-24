#include "RenderListViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include <spdlog/fmt/fmt.h>
#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"
#include "Tracy.hpp"

void RenderListWindowLayer::ListItemRenderItem(RenderCommand& item, int index) {
    bool isSelected = selected == index;
    std::string str = fmt::format("{}: {}", index, RenderCommand_GetString(item.m_id));
    if (ImGui::Selectable(str.c_str(), &isSelected)) { selected = index; }
}

void RenderListWindowLayer::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("[Development]", "Render List", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }

    if (ImGui::Begin("Render List", &this->show)) {
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
    ImGui::End();

    if (ImGui::Begin("Properties (Render List)", &this->show)) {
        if (selected != -1) {
            // Properties
            // We go through this loop again to display the current context, which we must generate through stepping
            // through each action

            // State
            Ref<Shader> stateShader = nullptr;
            Ref<RenderTexture> stateRenderTex = nullptr;
            Blazar::Rectangle stateViewport = Blazar::Rectangle();

            int index = 0;

            for (auto& x : Renderer::m_LastRenderQueue) {
                // Calculate state...
                if (x.m_id == RenderCommandID::SET_SHADER) { stateShader = std::get<Ref<Shader>>(x.data); }
                if (x.m_id == RenderCommandID::SET_VIEWPORT) { stateViewport = std::get<Blazar::Rectangle>(x.data); }
                if (x.m_id == RenderCommandID::SET_RENDERTEXTURE) {
                    stateRenderTex = std::get<Ref<RenderTexture>>(x.data);
                }

                bool displaySeperator = true;
                if (index == selected) {
                    // Display properties
                    switch (x.m_id) {
                        case RenderCommandID::SET_SHADER: {
                            ImGui::Text("Name: %s", stateShader != nullptr ? stateShader->name.c_str() : "None");
                        } break;

                        case RenderCommandID::SET_MAT4: {
                            auto d = std::get<std::pair<std::string, glm::mat4>>(x.data);
                            ImGui::Text("Uniform: %s", d.first.c_str());

                            ImGui::Text("Matrix:");
                            if (ImGui::BeginTable("Mat4", 4)) {
                                for (size_t i = 0; i < 16; i++) {
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%f", d.second[i % 4][i / 4]);
                                }

                                ImGui::EndTable();
                            }
                        } break;
                        default:
                            displaySeperator = false;
                            break;
                    }
                    if (displaySeperator) { ImGui::Separator(); }

                    // Display state
                    CImGUI_Header1("State at this moment");
                    ImGui::Spacing();
                    ImGui::Text("Shader: %s", stateShader != nullptr ? stateShader->name.c_str() : "None");
                    ImGui::Text("Render Texture: %d", stateRenderTex != nullptr ? stateRenderTex->GetId() : 0);
                    ImGui::Text("Current Viewport: %d x %d @ (%d, %d)", stateViewport.width, stateViewport.height,
                                stateViewport.x, stateViewport.y);
                    break;
                }

                index++;
            }
        }
    }
    ImGui::End();
}

#endif
