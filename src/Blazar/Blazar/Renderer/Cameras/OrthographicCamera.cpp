#include "bzpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OrthographicCamera.h"

namespace Blazar {

OrthographicCamera::OrthographicCamera(float zoom, float aspect) : m_Zoom(zoom) {
    RecalculatePerspectiveMatrix(aspect);
}

OrthographicCamera::OrthographicCamera(float zoom, Viewport& viewport) : m_Zoom(zoom) {
    RecalculatePerspectiveMatrix(m_Viewport->GetAspect());
}

void OrthographicCamera::SetPosition(const glm::vec3& cameraPosition) {
    m_CameraPosition = cameraPosition;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetViewport(Ref<Viewport> viewport) {
    m_Viewport = viewport;  // Copy
    RecalculatePerspectiveMatrix(m_Viewport->GetAspect());
}

void OrthographicCamera::SetZoom(float zoom) {
    m_Zoom = zoom;
    RecalculatePerspectiveMatrix(m_Viewport->GetAspect());
}

const Viewport& OrthographicCamera::GetViewport() { return *m_Viewport; }

float OrthographicCamera::GetZoom() { return m_Zoom; }

void OrthographicCamera::BeginPass() { RecalculatePerspectiveMatrix(m_Viewport->GetAspect()); }

void OrthographicCamera::RecalculatePerspectiveMatrix(float aspect) {
    float w = m_Zoom * aspect;
    float h = m_Zoom;
    float halfWidth = w / 2.0f;
    float halfHeight = h / 2.0f;
    m_MatProjection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
    m_MatViewProjection = m_MatProjection * m_MatView;
}

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_CameraPosition);
    m_MatView = glm::inverse(transform);
    m_MatViewProjection = m_MatProjection * m_MatView;
}

}  // namespace Blazar
