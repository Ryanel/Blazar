#include "RenderListViewer.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"
#include "Tracy.hpp"

void RenderListWindowLayer::ListItemRenderItem(RenderItem* item, int index) {
    std::string str;

    switch (item->type) {
        case RenderItemType::SET_VIEWPORT: {
            RenderItem_SetViewport* vp = (RenderItem_SetViewport*)item;
            str = fmt::format("{}: Set Viewport to {}x{} @ ({}, {})", index, vp->w, vp->h, vp->x, vp->y);
        } break;
        default:
            str = fmt::format("{}: {}", index, RenderItemString(item->type));
            break;
    }

    bool isSelected = selected == index;

    if (ImGui::Selectable(str.c_str(), &isSelected)) { selected = index; }
}

void RenderListWindowLayer::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "Render List", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }

    if (ImGui::Begin("Render List", &this->show)) {
        if (ImGui::Button("Clear Selection")) { selected = -1; }
        ImGui::SameLine();
        ImGui::Text("Count: %d", Renderer::m_LastFrameRenderQueue.size());

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
        if (selected != -1) {
            int i = 0;
            for (auto& x : Renderer::m_LastFrameRenderQueue) {
                if (i == selected) {
                    std::string str = fmt::format("Render Item {}: {}", i, RenderItemString(x->type));
                    ImGui::Text(str.c_str());
                    ImGui::Separator();

                    RenderItem_SetViewport* ri_viewport = nullptr;

                    switch (x->type) {
                        case RenderItemType::SET_VIEWPORT:
                            ri_viewport = (RenderItem_SetViewport*)x;
                            ImGui::Text("Pos: (%d, %d)", ri_viewport->x, ri_viewport->y);
                            ImGui::Text("Size: (%d, %d)", ri_viewport->w, ri_viewport->h);
                            break;
                        case RenderItemType::CLEAR_COLOR: {
                            RenderItem_ClearColor* clearColor = (RenderItem_ClearColor*)x;
                            float* colorPtr = &clearColor->r;
                            ImGui::ColorEdit4("Clear Color", colorPtr, ImGuiColorEditFlags_NoInputs);
                        } break;
                        case RenderItemType::SET_RENDERTEXTURE: {
                            RenderItem_SetRenderTexture* rt = (RenderItem_SetRenderTexture*)x;

                            if (rt->tex) {
                                ImGui::Text("RenderTexture <- %d", rt->tex->GetId());
                                float aspect = rt->tex->GetWidth() / rt->tex->GetHeight();

                                ImVec2 size = ImGui::GetContentRegionAvail();

                                if (rt->tex->GetWidth() >= rt->tex->GetHeight()) {
                                    size.y = std::min((size.x) / aspect, size.y);
                                } else {
                                    size.x = std::min((size.y) / aspect, size.x);
                                }

                                ImGui::Image((ImTextureID)rt->tex->m_ColorTexture->GetId(), size, ImVec2(0, 1),
                                             ImVec2(1, 0));
                            } else {
                                ImGui::Text("RenderTexture <- Null");
                            }
                        } break;

                        case RenderItemType::BIND_TEXTURE2D: {
                            RenderItem_BindTexture* item_bt = (RenderItem_BindTexture*)x;

                            if (item_bt->texture) {
                                ImGui::Text("ID %d", item_bt->texture->GetId());
                                ImGui::Text("Slot %d", item_bt->slot);
                                ImGui::Text("Size %d x %d", item_bt->texture->GetWidth(),
                                            item_bt->texture->GetHeight());
                                float aspect = item_bt->texture->GetWidth() / item_bt->texture->GetHeight();

                                ImVec2 size = ImGui::GetContentRegionAvail();

                                if (item_bt->texture->GetWidth() >= item_bt->texture->GetHeight()) {
                                    size.y = std::min((size.x) / aspect, size.y);
                                } else {
                                    size.x = std::min((size.y) / aspect, size.x);
                                }

                                ImGui::Image((ImTextureID)item_bt->texture->GetId(), size, ImVec2(0, 1), ImVec2(1, 0));
                            } else {
                                ImGui::Text("RenderTexture <- Null");
                            }
                        } break;
                        case RenderItemType::SET_SHADER: {
                            RenderItem_SetShader* ss = (RenderItem_SetShader*)x;
                            auto ogl_shader = std::dynamic_pointer_cast<Blazar::OpenGLShader>(ss->shader);
                            ImGui::Text("Name: %s", ogl_shader->name.c_str());
                            ImGui::Text("Number of Uniforms: %d", ogl_shader->m_UniformMap.size());
                            if (ImGui::BeginTable("##RenderProps", 2,
                                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                                      ImGuiTableFlags_SizingStretchSame)) {
                                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoResize, 1);
                                ImGui::TableSetupColumn("Uniform Name", ImGuiTableColumnFlags_NoResize, 3);
                                ImGui::TableHeadersRow();
                                for (auto& uniform : ogl_shader->m_UniformMap) {
                                    ImGui::TableNextRow();
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%d", uniform.second);
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", uniform.first.c_str());
                                }
                                ImGui::EndTable();
                            }
                        } break;

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
