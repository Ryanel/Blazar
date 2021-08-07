#include "bzpch.h"

#include "EntityViewer.h"

#include "Blazar/Application.h"
#include "Blazar/Editor/EditorCommand.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/IconsFontAwesome.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/SceneManager.h"
#include "Tracy.hpp"
#include "misc/cpp/imgui_stdlib.h"

#include "Blazar/Component/MeshComponent.h"
#include "Blazar/Component/NameComponent.h"
#include "Blazar/Component/TextureComponent.h"
#include "Blazar/Component/Transform.h"

namespace Blazar {
namespace Editor {

using namespace Blazar::Components;
EntityViewer::EntityViewer(Editor* editor, Entity entity)
    : EditorWindow(editor, fmt::format("Entity {}", (int)entity.entity()), "Entity", State::EDITOR_DOCKED),
      m_entity(entity) {
    m_width_min      = 400;
    m_width_expanded = 600;
}

void EntityViewer::render() {
    ZoneScoped;
    auto& app = Application::get();

    CImGUI_Header1(fmt::format("{} {{", m_entity.name().c_str()));

    // Draw all the components
    ImGui::Indent();
    ImGui::Text(ICON_FA_EDIT " Components");
    ImGui::Indent();
    m_footprint_size = 0;

    if (m_entity.has<Components::NameComponent>()) {
        render_component("Name", &m_entity.get<Components::NameComponent>());
    }
    if (m_entity.has<Components::Transform>()) {
        render_component("Transform", &m_entity.get<Components::Transform>());
    }
    if (m_entity.has<Components::MeshComponent>()) {
        render_component("Mesh", &m_entity.get<Components::MeshComponent>());
    }
    if (m_entity.has<Components::TextureComponent>()) {
        render_component("Texture", &m_entity.get<Components::TextureComponent>());
    }

    ImGui::Unindent();
    ImGui::Spacing();

    ImGui::Text(ICON_FA_CHART_PIE " Size");
    ImGui::Indent();

    ImGui::Text("Total Size: ");
    ImGui::Unindent();
    ImGui::SameLine();
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Alternate), "%d bytes", m_footprint_size);

    ImGui::Unindent();

    CImGUI_Header1("}");
}

void EntityViewer::render_component(std::string name, IComponent* component) {
    size_t size = component->size_of();
    // Render name
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Primary), "Component ");
    ImGui::SameLine();
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Secondary), name.c_str());
    ImGui::SameLine();
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Alternate), "(%d bytes)", size);

    component->inspect();
    ImGui::Separator();

    m_footprint_size += size;
}

void EntityViewer::on_close() {
    m_editor->perform(new CloseWindowWithArgument<EntityViewer, Entity>(
        m_editor, std::dynamic_pointer_cast<EntityViewer>(shared_from_this()), m_entity));
}

}  // namespace Editor
}  // namespace Blazar
