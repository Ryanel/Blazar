#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/ImGuiLayer.h"
#include "Blazar/Layer/Layer.h"

#include "Tracy.hpp"

namespace Blazar {
ImGuiLayer::ImGuiLayer() : Layer("ImGUI") {
    this->m_DebugName = "ImGUI";
    m_UpdatePath = LayerUpdatePath::ImGui;
}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
    ZoneScoped;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    imgui_font_normal = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/Roboto-Medium.ttf", 16);
    imgui_font_big = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/Roboto-Medium.ttf", 18);
    imgui_font_bigger = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/Roboto-Medium.ttf", 24);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 4;
    style.WindowPadding.x = 2;
    style.WindowPadding.y = 2;
    style.FrameRounding = 4;
    style.FramePadding.x = 4;
    style.FramePadding.y = 4;
    style.WindowTitleAlign.x = 0.50f;
    style.CellPadding.x = 2;
    style.CellPadding.y = 4;
    style.ItemSpacing.y = 4;
    style.ScrollbarSize = 8;
    style.TabRounding = 2;
    style.GrabRounding = 6;
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Border] = ImVec4(0.22f, 0.22f, 0.22f, 0.50f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.54f);
    colors[ImGuiCol_Separator] = ImVec4(0.16f, 0.16f, 0.16f, 0.50f);
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.34f, 0.63f, 0.86f);
    colors[ImGuiCol_TabActive] = ImVec4(0.08f, 0.42f, 0.86f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.29f, 0.47f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.35f, 0.63f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.26f, 0.26f, 0.29f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.47f, 0.47f, 0.47f, 0.06f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.39f, 0.39f, 0.39f, 0.27f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.42f, 0.42f, 0.42f, 0.37f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.71f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.49f);

    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetached() {}

void ImGuiLayer::Begin() {
    ZoneScoped;
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
    io.DeltaTime = app.m_deltaTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End(bool draw) {
    ZoneScoped;
    ImGui::Render();

    if (draw) { ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

}  // namespace Blazar
#endif
