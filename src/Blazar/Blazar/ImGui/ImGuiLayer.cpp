#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/ImGuiLayer.h"
#include "Blazar/Layer/Layer.h"

namespace Blazar {
ImGuiLayer::ImGuiLayer() : Layer("ImGUI") { this->m_DebugName = "ImGUI"; }

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
    BLAZAR_PROFILE_FUNCTION();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 4;
    style.WindowPadding.x = 2;
    style.WindowPadding.y = 2;
    style.FrameRounding = 0;
    style.FramePadding.x = 4;
    style.FramePadding.y = 4;
    style.WindowTitleAlign.x = 0.50f;

    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetached() {}
void ImGuiLayer::OnUpdate(Blazar::Timestep ts) {}
void ImGuiLayer::OnEvent(Events::Event& ev) {}

void ImGuiLayer::Begin() {
    BLAZAR_PROFILE_FUNCTION();
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
    io.DeltaTime = app.m_deltaTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End(bool draw) {
    BLAZAR_PROFILE_FUNCTION();
    ImGui::Render();

    if (draw) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

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