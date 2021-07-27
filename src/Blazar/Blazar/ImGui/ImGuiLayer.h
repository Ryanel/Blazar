#pragma once

#ifdef BLAZAR_IMGUI_ENABLED
namespace Blazar {
class ImGuiLayer {
   public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void OnAttach();
    void OnDetached();

    void Begin();
    void End(bool draw = true);

   private:
    float m_Time = 0;
};
}  // namespace Blazar

#endif
