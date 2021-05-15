#pragma once

namespace Blazar {
enum class RendererAPI { None = 0, OpenGL = 1 };

class Renderer {
   public:
    static inline RendererAPI CurrentAPI() { return s_RendererAPI; }

   private:
    static RendererAPI s_RendererAPI;
};

};  // namespace Blazar
