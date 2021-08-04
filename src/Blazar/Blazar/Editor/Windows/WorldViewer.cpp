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

WorldViewer::WorldViewer() : EditorWindow("World Viewer", "World", EditorWindowState::EDITOR_DOCKED) {
    m_texgear = Texture2D::Load("/Editor/Textures/gear.png");
}

void WorldViewer::render(Editor* editor) {
    ZoneScoped;
    auto& app = Application::get();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear Selection")) { m_selected_entity = entt::null; }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    auto currentScene = app.scenemanager()->current();

    if (currentScene != nullptr) {
        auto& registry = currentScene->registry();
        if (ImGui::BeginListBox("##EntityList", ImGui::GetContentRegionAvail())) {
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

}  // namespace Editor
}  // namespace Blazar
