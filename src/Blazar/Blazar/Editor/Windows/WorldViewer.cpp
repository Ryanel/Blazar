#include "bzpch.h"

#include "WorldViewer.h"

#include "Blazar/Application.h"
#include "Blazar/Component/NameComponent.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"

#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
WorldViewer::WorldViewer() : EditorWindow("World Viewer") { m_texgear = Texture2D::Load("/Editor/Textures/gear.png"); }
void WorldViewer::RenderWindow() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "World Viewer", "", this->m_active, true);

    auto& app = Application::Get();

    if (!this->m_active) { return; }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("World Viewer", &this->m_active)) {
        isFocused = ImGui::IsWindowFocused();

        ImGui::PushStyleColor(
            ImGuiCol_ChildBg,
            ImGui::GetStyleColorVec4((isFocused || isTitleFocused) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg));
        if (ImGui::BeginChild("##WorldViewerTop", ImVec2(0, 30))) {
            isTitleFocused = ImGui::IsWindowFocused();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);

            if (ImGui::ImageButton(m_texgear->data()->imgui_id(), ImVec2(16.0f, 16.0f), ImVec2(0.0f, 1.0f),
                                   ImVec2(1, 0))) {
                m_options_open = !m_options_open;
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear Selection")) { m_selected_entity = entt::null; }
            ImGui::SameLine();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        auto currentScene = app.SceneManager()->GetCurrentScene();

        if (currentScene != nullptr) {
            auto& registry = currentScene->registry();
            ImGui::Text("Scene Name: %s", currentScene->name().c_str());

            if (ImGui::BeginListBox("##EntityList", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
                registry.each([&registry, this](entt::entity ent) {
                    Entity e(&registry, ent);

                    std::string entityName;
                    if (e.has<Components::NameComponent>()) {
                        auto name  = e.get<Components::NameComponent>();
                        entityName = name.name;
                    } else {
                        entityName = fmt::format("Entity {}", (int)e.entity());
                    }

                    bool selected = m_selected_entity == e.entity();

                    if (ImGui::Selectable(entityName.c_str(), &selected)) { m_selected_entity = e.entity(); }
                });

                ImGui::EndListBox();
            }
        } else {
            ImGui::Text("No scene active");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    if (m_opt_show_properties_always || m_selected_entity != entt::null) {
        bool should_stay_open = true;
        if (ImGui::Begin("Entity Properties", &should_stay_open)) {
            if (m_selected_entity != entt::null) {
                Entity      e(&app.SceneManager()->GetCurrentScene()->registry(), m_selected_entity);
                std::string entityName;
                if (e.has<Components::NameComponent>()) {
                    auto name  = e.get<Components::NameComponent>();
                    entityName = name.name;
                } else {
                    entityName = fmt::format("Entity {}", (int)e.entity());
                }

                CImGUI_Header1(entityName);

                // Inspector logic
            }
        }
        ImGui::End();
    }
}

}  // namespace Editor
}  // namespace Blazar
