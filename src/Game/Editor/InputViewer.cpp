#include "InputViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"
#include "Tracy.hpp"

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

#endif
