#pragma once

#ifdef BLAZAR_IMGUI_ENABLED
namespace Blazar {
class ImGuiSystem {
   public:
    ImGuiSystem();
    virtual ~ImGuiSystem();

    void init();
    void destroy();

    void begin_draw();
    void end_draw(bool draw = true);

   private:
    float m_Time = 0;
};
}  // namespace Blazar

#endif
