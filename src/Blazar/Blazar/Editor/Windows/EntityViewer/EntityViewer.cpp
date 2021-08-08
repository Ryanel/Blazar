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
    using namespace entt;
    ZoneScoped;
    auto& app        = Application::get();
    m_footprint_size = 0;
    CImGUI_Header1(fmt::format("{} {{", m_entity.name().c_str()));

    // Draw all the components
    ImGui::Indent();
    ImGui::Text(ICON_FA_EDIT " Components");
    ImGui::Indent();

    auto& registry = *m_entity.registry();

    registry.visit(m_entity.entity(), [&](const entt::type_info component_info) {
        const auto component_type = entt::resolve(component_info);
        auto       friendly_name  = component_type.prop("friendly_name"_hs).value().cast<const char*>();
        bool       inspector      = false;

        m_footprint_size += component_type.size_of();

        ImGui::TextColored(EditorUIColor(EditorUICustomColor::Primary), "Component ");
        ImGui::SameLine();
        ImGui::TextColored(EditorUIColor(EditorUICustomColor::Secondary), "%s", friendly_name);

        if (ImGui::IsItemHovered()) {
            auto description = component_type.prop("description"_hs);

            if (description) {
                ImGui::BeginTooltip();
                ImGui::Text("%s", description.value().cast<const char*>());
                ImGui::EndTooltip();
            }
        }

        auto any = component_type.func("get"_hs).invoke({}, std::ref(registry), m_entity.entity());

        if (any) {
            if (auto inspect = component_type.func("inspect"_hs); inspect) {
                inspect.invoke(any);
                inspector = true;
            }
        }

        if (!inspector) { ImGui::Text("No inspector!"); }
    });

    ImGui::Unindent();

    ImGui::Text(ICON_FA_CHART_PIE " Size");
    ImGui::Indent();
    ImGui::Text("Total Size: ");
    ImGui::Unindent();
    ImGui::SameLine();
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Alternate), "%d bytes", m_footprint_size);
    ImGui::Unindent();
    CImGUI_Header1("}");
}

void EntityViewer::on_close() {
    m_editor->perform(new CloseWindowWithArgument<EntityViewer, Entity>(
        m_editor, std::dynamic_pointer_cast<EntityViewer>(shared_from_this()), m_entity));
}

}  // namespace Editor
}  // namespace Blazar
