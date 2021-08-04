#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED

#include "LogWindow.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/fmt/ostr.h>

#include "Blazar/Application.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
static const char* const spdlog_level_names[] = {"Trace", "Debug", "Info", "Warn", "Error", "Critical", "Off"};

void LogWindow::render(Editor* editor) {
    ZoneScoped;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("Log", &this->m_Show)) {
        if (ImGui::BeginChild("##LogTop", ImVec2(0, 28))) {
            ImGui::Checkbox("Show options", &this->m_Options);

            ImGui::SameLine();

            if (ImGui::BeginCombo("Filter Level", spdlog_level_names[m_filterSeverity])) {
                for (size_t i = 0; i < spdlog::level::n_levels; i++) {
                    if (ImGui::Selectable(spdlog_level_names[i])) { m_filterSeverity = (spdlog::level::level_enum)i; }
                }

                ImGui::EndCombo();
            }
        }
        ImGui::EndChild();

        if (m_Options) {
            ImGui::Checkbox("Scroll Log to Bottom", &this->m_ScrollToBottom);
            ImGui::Text("Current Entries: %d", Log::s_LogEntries.size());
            ImGui::DragInt("Entries to Store", &Log::s_MaxLogEntries, 1.0f, 1, 10000000);
            ImGui::DragInt("Entries to Display", &m_EntriesToShow, 1.0f, 0, 10000000);

            if (ImGui::Button("Clear Log")) { Log::s_LogEntries.clear(); }

            if (m_EntriesToShow > Log::s_MaxLogEntries) { m_EntriesToShow = Log::s_MaxLogEntries; }
        }

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2, 2));
        if (ImGui::BeginTable("log#entry_table", 4,
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY)) {
            ZoneScopedN("Log Entry Display");
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_NoResize, 1.0);
            ImGui::TableSetupColumn("Severity", ImGuiTableColumnFlags_None, 1.0);
            ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_None, 1.5);
            ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_None, 9.0);
            ImGui::TableHeadersRow();

            size_t i = Log::s_LogEntries.size() > m_EntriesToShow ? (Log::s_LogEntries.size() - m_EntriesToShow) : 0;
            for (; i < Log::s_LogEntries.size(); i++) {
                auto& entry = Log::s_LogEntries[i];

                if (entry.details.level >= m_filterSeverity) { DisplayEntry(entry); }
            }

            if (m_ScrollToBottom) { ImGui::SetScrollHereY(); }
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();

    ImGui::PopStyleVar();

    if (!m_Show) { editor->close_window(this); }
}

void LogWindow::DisplayEntry(log_entry& entry) {
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

    ImGui::Text(spdlog_level_names[entry.details.level]);

    ImGui::TableNextColumn();  // Time
    ImGui::Text("%s", entry.time_fmt.c_str());

    ImGui::TableNextColumn();  // Message
    ImGui::Text("%s", entry.msg.c_str());

    ImGui::PopStyleColor(1);
}

}  // namespace Editor
}  // namespace Blazar

#endif
