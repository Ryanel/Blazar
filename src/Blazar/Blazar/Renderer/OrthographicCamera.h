#pragma once

#include "Blazar/Renderer/Camera.h"
#include "Blazar/Renderer/Viewport.h"

namespace Blazar {

// TODO: Add caching of matricies

/// Camera with no perspective.
class OrthographicCamera : public Camera {
   public:
    /// Creates an orthographic camera with a center X and Y position, extending width/2 and height/2 in each axis.
    OrthographicCamera(float zoom, float aspect);
    OrthographicCamera(float zoom, Viewport& viewport);
    
    virtual void SetPosition(const glm::vec3& cameraPosition) override;
    
    void SetViewport(Viewport& viewport);
    const Viewport& GetViewport();

    void SetZoom(float zoom);
    float GetZoom();

   private:
    float m_Zoom;
    float m_Aspect;
    Viewport m_Viewport;
    void RecalculatePerspectiveMatrix();
    void RecalculateViewMatrix();
};

}  // namespace Blazar
