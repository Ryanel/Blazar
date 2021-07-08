#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "OpenGLRendererAPI.h"

#include <TracyOpenGL.hpp>

namespace Blazar {

void OpenGLRendererAPI::Clear() {
    TracyGpuZoneS("Clear", 12);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
    TracyGpuZoneS("SetClearColor", 12);
    glClearColor(r,g,b,a);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
    TracyGpuZoneS("DrawIndexed", 12);
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
    TracyGpuZoneS("SetViewport", 12);
    glViewport(x, y, width, height);
}

}  // namespace Blazar
