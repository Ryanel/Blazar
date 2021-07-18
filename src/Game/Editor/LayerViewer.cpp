#include "LayerViewer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"
#include "Tracy.hpp"

void LayerEditorWindow::RenderTable(LayerUpdatePath path) {
    if (ImGui::BeginTable("##LayerList", 2, ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableSetupColumn("Layer Order", 0, 1.0f);
        ImGui::TableSetupColumn("Layer Name", 0, 3.0f);
        ImGui::TableHeadersRow();

        int index = 0;
        for (auto& x : Application::Get().m_LayerStack) {
            if (((int)x->m_UpdatePath & (int)path) == 0) { continue; }
            ImGui::TableNextColumn();
            ImGui::Text("%d", index);
            ImGui::TableNextColumn();
            ImGui::Text("%s", x->GetName().c_str());

            index++;
        }
        ImGui::EndTable();
    }
}

void LayerEditorWindow::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("[Development]", "Layers", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }

    if (ImGui::Begin("Layer Viewer", &this->show)) {
        CImGUI_Header1("Layers");
        CImGUI_Header2("Update");
        RenderTable(LayerUpdatePath::Update);
        CImGUI_Header2("Render");
        RenderTable(LayerUpdatePath::Render);
        CImGUI_Header2("ImGui");
        RenderTable(LayerUpdatePath::ImGui);
    }
    ImGui::End();
}

#endif
