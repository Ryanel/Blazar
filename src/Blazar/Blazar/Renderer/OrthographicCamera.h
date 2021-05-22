#pragma once

#include "Blazar/Renderer/Camera.h"
#include "Blazar/Renderer/Viewport.h"

namespace Blazar {

// TODO: Add caching of matricies

/// Camera with no perspective.
class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(float zoom, float aspect);
    OrthographicCamera(float zoom, Viewport& viewport);
    
    /// Moves the camera to the specified cameraPosition
    virtual void SetPosition(const glm::vec3& cameraPosition) override;
    
    virtual void SetViewport(Viewport& viewport) override;
    virtual const Viewport& GetViewport() override;

    void SetZoom(float zoom); ///< Set the current zoom level
    float GetZoom(); ///< Get the current zoom level

   private:
    float m_Zoom;
    float m_Aspect;
    Viewport m_Viewport;
    void RecalculatePerspectiveMatrix();
    void RecalculateViewMatrix();
};

}  // namespace Blazar
