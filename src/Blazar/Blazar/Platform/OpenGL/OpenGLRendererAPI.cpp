#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "OpenGLRendererAPI.h"

namespace Blazar {

void OpenGLRendererAPI::Clear() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

}  // namespace Blazar
