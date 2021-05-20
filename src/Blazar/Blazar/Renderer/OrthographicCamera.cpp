#include "bzpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Blazar/Renderer/OrthographicCamera.h"

namespace Blazar {

OrthographicCamera::OrthographicCamera(float x, float y, float zoom, float aspect) {
    float w = zoom * aspect;
    float h = zoom;
    float halfWidth = w / 2.0f;
    float halfHeight = h / 2.0f;
    m_MatProjection = glm::ortho(x - halfWidth, x + halfWidth, y - halfHeight, y + halfHeight, -1.0f, 1.0f);
}


void OrthographicCamera::SetPosition(const glm::vec3& cameraPosition) {
    m_CameraPosition = cameraPosition;
    RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition);

    m_MatView = glm::inverse(transform);
    m_MatViewProjection = m_MatProjection * m_MatView;
}

}  // namespace Blazar
