#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>

#include <Blazar/Input/Keymap.h>
#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
using namespace Blazar;

class DebugEventLayer : public Blazar::Layer {
   public:
    DebugEventLayer() : Layer("Debug Event Layer") {}
    void OnUpdate() override {
        if (Input::KeyPressed(BLAZAR_KEY_SEMICOLON)) { LOG_GAME_TRACE("KEY PRESSED"); }
        if (Input::MouseButtonPressed(0)) { LOG_GAME_TRACE("MOUSE PRESSED"); }
        if (Input::GetMouseX() > 1280.0f) { LOG_GAME_TRACE("MOUSE X > 1280"); }
    }
    void OnEvent(Blazar::Event& event) override {}
};

class ImGUIFPSWindowLayer : public Blazar::Layer {
   public:
    ImGUIFPSWindowLayer() : Layer("ImGUI: FPS") {}

    void OnImGUIRender() override {
        ImGuiIO& io = ImGui::GetIO();
        if (!(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)) {
            float width = 128;
            float padding = 8;
            float screenWidth = Application::Get().GetWindow().GetWidth();

            float x = screenWidth - width - padding;
            float y = padding;

            ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(width, 48), ImGuiCond_Always);
        }

        float delta = Application::Get().m_deltaTime;

        bool frameRateOpen = true;
        if (ImGui::Begin("Framerate", &frameRateOpen,
                         ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove)) {
            ImGui::Text("FPS: %0.2f", 1.0f / delta);
            ImGui::Text("Delta: %0.2f ms", (delta * 1000.0f));
        }

        ImGui::End();
    }
};

class Game : public Blazar::Application {
   public:
    Game() {
        LOG_GAME_TRACE("Initialising (Constructor)");
        PushLayer(new DebugEventLayer());
        PushLayer(new ImGUIFPSWindowLayer());
    }

    ~Game() { LOG_GAME_INFO("Exiting (Destructor)"); }
};

Blazar::Application* Blazar::CreateApplication() { return new Game(); }
