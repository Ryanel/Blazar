#include "bzpch.h"

#include "Blazar/ImGui/IconsFontAwesome.h"
#include "Editor.h"
#include "EditorWindow.h"
#include "imgui.h"
#include "imgui_internal.h"
int rotation_start_index;

void ImRotateStart() { rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size; }

ImVec2 ImRotationCenter() {
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);  // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++) l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);  // or use _ClipRectStack?
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return {l.x - r.x, l.y - r.y}; }

void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter()) {
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++) buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void Blazar::Editor::EditorWindow::draw_editor_window() {
    if (m_useCustomWindow) {
        render();
    } else {
        if (m_state != State::FREE_FLOATING) {
            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));

            if (m_editorScrollToWindow) {
                ImGui::SetScrollHereX(0.0f);
                m_editorScrollToWindow = false;
            }

            if (ImGui::BeginChild(m_name.c_str(), ImVec2(m_width, m_height), true, ImGuiWindowFlags_MenuBar)) {
                if (m_state != State::DOCKED_MINIMIZED) {
                    if (ImGui::BeginMenuBar()) {
                        if (ImGui::BeginMenu(m_title.c_str())) {
                            if (ImGui::MenuItem("Exit", "")) { m_windowOpen = false; }
                            ImGui::EndMenu();
                        }

                        ImGui::Separator();
                        show_close_control();
                        ImGui::EndMenuBar();
                    }
                    render();
                } else {
                    // For minimized windows, draw a transparent button to allow unminimizing
                    ImVec4 trans = ImVec4(0, 0, 0, 0);
                    ImGui::PushStyleColor(ImGuiCol_Button, trans);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, trans);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, trans);
                    if (ImGui::Button("", ImVec2(ImGui::GetContentRegionAvail()))) { unminimize(); }
                    ImGui::PopStyleColor(3);

                    // Draw the text rotated to allow more room
                    ImRotateStart();
                    auto textSize = ImGui::CalcTextSize(m_title.c_str());

                    ImGui::SetCursorPosX((-textSize.x / 2) + m_width / 2);
                    ImGui::SetCursorPosY(m_height / 2);
                    ImRect clip_rect = ImGui::GetCurrentWindow()->ClipRect;
                    clip_rect.Min.x -= textSize.x;
                    clip_rect.Max.x += textSize.x;

                    ImGui::PushClipRect(clip_rect.Min, clip_rect.Max, false);
                    ImGui::Text(m_title.c_str());
                    ImRotateEnd(3.141593f);
                }
            }

            ImGui::EndChild();
            ImGui::PopStyleColor();
        } else {
            if (ImGui::Begin(m_name.c_str(), &m_windowOpen)) { render(); }
            ImGui::End();
        }

        if (!m_windowOpen) { m_editor->close_window(this); }
    }
}

void Blazar::Editor::EditorWindow::show_close_control() {
    float x_cursor = ImGui::GetCursorPosX();
    if (m_state != State::FREE_FLOATING) {
        int amount = 24 + 32;

        if (m_allowsExpansion) { amount += 32; }
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - amount);

        ImVec4 trans = ImVec4(0, 0, 0, 0);
        ImGui::PushStyleColor(ImGuiCol_Button, trans);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, trans);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, trans);

        if (ImGui::Button(ICON_FA_MINUS)) { m_state = State::DOCKED_MINIMIZED; }
        if (m_allowsExpansion) {
            if (m_state == State::EDITOR_DOCKED) {
                if (ImGui::Button(ICON_FA_EXPAND_ALT)) {
                    m_state                = State::DOCKED_EXPANDED;
                    m_editorScrollToWindow = true;
                }
            } else {
                if (ImGui::Button(ICON_FA_COMPRESS_ALT)) { m_state = State::EDITOR_DOCKED; }
            }
        }
        if (ImGui::Button(ICON_FA_TIMES)) { m_windowOpen = false; }

        ImGui::PopStyleColor(3);
        ImGui::SetCursorPosX(x_cursor);
    }
}

void Blazar::Editor::EditorWindow::unminimize() {
    m_state = State::EDITOR_DOCKED;
    m_editor->window_move_end(this);
    m_editorScrollToWindow = true;
}
