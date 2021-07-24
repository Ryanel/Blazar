#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLContext.h"
#include "Blazar/Renderer/Primitives/Buffer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "OpenGLRendererAPI.h"

#include <TracyOpenGL.hpp>

namespace Blazar {

void OpenGLRendererAPI::Clear() {
    // TracyGpuZone("Clear");
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
    // TracyGpuZone("SetClearColor");
    glClearColor(r, g, b, a);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
    // TracyGpuZone("DrawIndexed");
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
    // TracyGpuZone("SetViewport");
    glViewport(x, y, width, height);
}

}  // namespace Blazar
