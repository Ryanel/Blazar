#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/IconsFontAwesome.h"
#include "Blazar/ImGui/ImGuiSystem.h"

#include "Tracy.hpp"

namespace Blazar {
ImGuiSystem::ImGuiSystem() {}
ImGuiSystem::~ImGuiSystem() {}

void ImGuiSystem::init() {
    ZoneScoped;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    imgui_font_normal = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/CascadiaMono.ttf", 16.0f);
    ImFontConfig config;
    config.MergeMode                   = true;
    config.PixelSnapH                  = true;
    static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    imgui_font_normal =
        io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/fa-solid-900.ttf", 16.0f, &config, icon_ranges);
    imgui_font_big    = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/CascadiaMono.ttf", 24.0f);
    imgui_font_bigger = io.Fonts->AddFontFromFileTTF("Contents/Editor/Fonts/CascadiaMono.ttf", 32.0f);

    io.Fonts->Build();

    ImGuiStyle& style              = ImGui::GetStyle();
    style.WindowBorderSize         = 0.0f;
    style.ChildBorderSize          = 0.0f;
    style.ChildRounding            = 4;
    style.WindowRounding           = 4;
    style.WindowPadding.x          = 2;
    style.WindowPadding.y          = 2;
    style.FrameRounding            = 4;
    style.FramePadding.x           = 4;
    style.FramePadding.y           = 4;
    style.WindowTitleAlign.x       = 0.00f;
    style.CellPadding.x            = 2;
    style.CellPadding.y            = 4;
    style.ItemSpacing.y            = 4;
    style.ItemSpacing.x            = 4;
    style.ScrollbarSize            = 12;
    style.TabRounding              = 2;
    style.GrabRounding             = 6;
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text]               = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
    colors[ImGuiCol_Tab]                = ImVec4(0.12f, 0.34f, 0.63f, 0.86f);
    colors[ImGuiCol_TabActive]          = ImVec4(0.08f, 0.42f, 0.86f, 1.00f);
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.15f, 0.29f, 0.47f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.35f, 0.63f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]      = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]  = ImVec4(0.26f, 0.26f, 0.29f, 1.00f);
    colors[ImGuiCol_TableBorderLight]   = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt]      = ImVec4(0.47f, 0.47f, 0.47f, 0.06f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.08f, 0.09f, 0.10f, 0.27f);
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.71f);
    colors[ImGuiCol_TableRowBg]         = ImVec4(0.09f, 0.09f, 0.09f, 0.49f);
    colors[ImGuiCol_ChildBg]            = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_WindowBg]           = ImVec4(0.12f, 0.13f, 0.14f, 1.00f);
    colors[ImGuiCol_Border]             = ImVec4(0.19f, 0.19f, 0.19f, 0.50f);
    colors[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.10f, 0.10f, 0.10f, 0.72f);
    colors[ImGuiCol_MenuBarBg]          = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]        = ImVec4(0.12f, 0.13f, 0.14f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]      = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[ImGuiCol_Button]             = ImVec4(0.22f, 0.22f, 0.22f, 0.75f);
    colors[ImGuiCol_Separator]          = ImVec4(0.29f, 0.29f, 0.29f, 0.75f);
    colors[ImGuiCol_TitleBg]            = ImVec4(0.22f, 0.22f, 0.22f, 0.54f);
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.27f, 0.27f, 0.27f, 0.54f);

    Application& app = Application::get();
    io.DisplaySize   = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiSystem::destroy() {}

void ImGuiSystem::begin_draw() {
    ZoneScoped;
    ImGuiIO&     io  = ImGui::GetIO();
    Application& app = Application::get();
    io.DisplaySize   = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
    io.DeltaTime     = app.m_deltaTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiSystem::end_draw(bool draw) {
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
