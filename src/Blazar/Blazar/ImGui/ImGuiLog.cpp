#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/fmt/ostr.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/ImGuiLog.h"
#include "Blazar/ImGui/ImGuiLayer.h"
#include "Blazar/Layer/Layer.h"

#include "Tracy.hpp"

namespace Blazar {

void ImGUILogWindowLayer::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "Log", "", this->m_Show, true);

    if (!m_Show) { return; }

    if (ImGui::Begin("Log", &this->m_Show)) {
        ImGui::Checkbox("Show options", &this->m_Options);

        ImGui::SameLine();

        if (ImGui::BeginCombo("Filter Level", spdlog::level::to_string_view(m_filterSeverity).data())) {
            for (size_t i = 0; i < spdlog::level::n_levels; i++) {
                if (ImGui::Selectable(spdlog::level::to_string_view((spdlog::level::level_enum)i).data())) {
                    m_filterSeverity = (spdlog::level::level_enum)i;
                }
            }

            ImGui::EndCombo();
        }

        if (m_Options) {
            ImGui::Checkbox("Scroll Log to Bottom", &this->m_ScrollToBottom);
            ImGui::Text("Current Entries: %d", Log::s_LogEntries.size());
            ImGui::DragInt("Entries to Store", &Log::s_MaxLogEntries, 1.0f, 1, 10000000);
            ImGui::DragInt("Entries to Display", &m_EntriesToShow, 1.0f, 0, 10000000);

            if (ImGui::Button("Clear Log")) { Log::s_LogEntries.clear(); }

            if (m_EntriesToShow > Log::s_MaxLogEntries) { m_EntriesToShow = Log::s_MaxLogEntries; }


        }

        ImGui::Separator();


        if (ImGui::BeginTable("log#entry_table", 4,
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY)) {
            ZoneScopedN("Log Entry Display");
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_NoResize, 1.0);
            ImGui::TableSetupColumn("Severity", ImGuiTableColumnFlags_None, 1.0);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_None, 2.0);
            ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_None, 8.0);
            ImGui::TableHeadersRow();

            size_t i = Log::s_LogEntries.size() > m_EntriesToShow ? (Log::s_LogEntries.size() - m_EntriesToShow) : 0;
            for (; i < Log::s_LogEntries.size(); i++) {
                auto& entry = Log::s_LogEntries[i];

                if (entry.details.level >= m_filterSeverity) { DisplayEntry(entry); }
            }
            
            if (m_ScrollToBottom) { ImGui::SetScrollHereY(); }
            ImGui::EndTable();
        }
        
    }
    ImGui::End();
}

void ImGUILogWindowLayer::DisplayEntry(log_entry& entry) {
    switch (entry.details.level) {
        case spdlog::level::err:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 0, 0)));
            break;
        case spdlog::level::warn:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 0)));
            break;
        case spdlog::level::critical:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(200, 0, 0)));
            break;
        case spdlog::level::trace:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(200, 200, 200)));
            break;
        case spdlog::level::info:
        default:
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
            break;
    }

    ImGui::TableNextRow();

    ImGui::TableNextColumn();  // Source
    ImGui::Text("%s", entry.details.logger_name.data());

    ImGui::TableNextColumn();  // Severity

    ImGui::Text(spdlog::level::to_string_view(entry.details.level).data());

    ImGui::TableNextColumn();  // Time
    ImGui::Text("%s", entry.time_fmt.c_str());

    ImGui::TableNextColumn();  // Message
    ImGui::Text("%s", entry.msg.c_str());

    ImGui::PopStyleColor(1);
}

}  // namespace Blazar

#endif