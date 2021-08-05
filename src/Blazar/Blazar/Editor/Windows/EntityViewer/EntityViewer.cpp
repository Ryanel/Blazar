#include "bzpch.h"

#include "EntityViewer.h"

#include "Blazar/Application.h"
#include "Blazar/Component/NameComponent.h"
#include "Blazar/Editor/EditorCommand.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

EntityViewer::EntityViewer(Editor* editor, Entity entity)
    : EditorWindow(editor, fmt::format("Entity {}", (int)entity.entity()), "Entity", State::EDITOR_DOCKED),
      m_entity(entity) {}

void EntityViewer::render() {
    ZoneScoped;
    auto& app = Application::get();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear Selection")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Text(m_entity.name().c_str());
}

}  // namespace Editor
}  // namespace Blazar
