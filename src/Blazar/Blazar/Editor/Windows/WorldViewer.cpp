#include "bzpch.h"

#include "WorldViewer.h"

#include "Blazar/Application.h"
#include "Blazar/Component/NameComponent.h"
#include "Blazar/Editor/EditorCommand.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"
#include "EntityViewer/EntityViewer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

class EntitySelectionCommand : public EditorCommand {
   public:
    EntitySelectionCommand(Editor* e, Ref<WorldViewer> window, entt::entity selected, entt::entity prev)
        : EditorCommand(e), m_window(window), m_selected(selected), m_prev_selected(prev) {}
    void             undo() { m_window->m_selected_entity = m_prev_selected; }
    void             execute() { m_window->m_selected_entity = m_selected; }
    std::string      name() { return "Entity Selection"; }
    Ref<WorldViewer> m_window;
    entt::entity     m_selected;
    entt::entity     m_prev_selected;
};

WorldViewer::WorldViewer(Editor* editor) : EditorWindow(editor, "World Viewer", "World", State::EDITOR_DOCKED) {}

void WorldViewer::render() {
    ZoneScoped;
    auto& app = Application::get();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Clear Selection")) {
                m_editor->perform(new EntitySelectionCommand(m_editor,
                                                             std::dynamic_pointer_cast<WorldViewer>(shared_from_this()),
                                                             entt::null, m_selected_entity));
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    auto currentScene = app.scenemanager()->current();

    if (currentScene == nullptr) {
        ImGui::Text("No scene active");
        return;
    }

    auto& registry = currentScene->registry();
    if (ImGui::BeginListBox("##EntityList", ImGui::GetContentRegionAvail())) {
        registry.each([&registry, this](entt::entity ent) {
            Entity       e(&registry, ent);
            bool         selected = m_selected_entity == e.entity();
            std::string& name     = e.name();
            if (ImGui::Selectable(name.c_str(), &selected)) {
                if (selected) {
                    m_editor->perform(new OpenWindowWithArgument<EntityViewer, Entity>(m_editor, e));
                } else {
                    m_editor->perform(
                        new EntitySelectionCommand(m_editor, std::dynamic_pointer_cast<WorldViewer>(shared_from_this()),
                                                   e.entity(), m_selected_entity));
                }
            }
        });

        ImGui::EndListBox();
    }
}

}  // namespace Editor
}  // namespace Blazar
