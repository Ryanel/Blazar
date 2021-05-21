#pragma once

#include <glm/glm.hpp>

namespace Blazar {

/// Represents a Generic Camera
class Camera {
   public:
    virtual ~Camera() {}

    virtual void SetPosition(const glm::vec3& cameraPosition) = 0;
    const glm::vec3& GetPosition() const { return m_CameraPosition; }

    const virtual glm::mat4& GetProjection() const { return m_MatProjection; }
    const virtual glm::mat4& GetView() const { return m_MatView; }
    const virtual glm::mat4& GetViewProjection() const { return m_MatViewProjection; }

   protected:
    glm::mat4 m_MatProjection;
    glm::mat4 m_MatView;
    glm::mat4 m_MatViewProjection;

    glm::vec3 m_CameraPosition;
};

}  // namespace Blazar
