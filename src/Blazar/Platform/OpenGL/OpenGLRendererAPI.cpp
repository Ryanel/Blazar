#include "bzpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

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

}  // namespace Blazar
