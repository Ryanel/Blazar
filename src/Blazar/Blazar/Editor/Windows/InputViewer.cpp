#include "bzpch.h"

#include "InputViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void InputEditorWindow::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("[Development]", "Inputs", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }

    if (ImGui::Begin("Input Viewer", &this->show)) {
        CImGUI_Header1("Pressed Inputs:");
        if (ImGui::BeginTable("##InputList", 2)) {
            ImGui::TableSetupColumn("Input Name");
            ImGui::TableSetupColumn("Input Keycode");
            ImGui::TableHeadersRow();
            for (size_t i = BLAZAR_KEY_MIN; i < BLAZAR_KEY_MAX; i++) {
                if (Input::Key(i)) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%c", i);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", i);
                }
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

}  // namespace Editor
}  // namespace Blazar

#endif
