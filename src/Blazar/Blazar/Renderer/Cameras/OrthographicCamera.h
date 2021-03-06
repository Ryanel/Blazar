#pragma once

#include "Blazar/Renderer/Cameras/Camera.h"

namespace Blazar {

// TODO: Add caching of matricies

/// Camera with no perspective.
class OrthographicCamera : public Camera {
   public:
    OrthographicCamera(float zoom, float aspect);
    OrthographicCamera(float zoom, Viewport& viewport);

    /// Moves the camera to the specified cameraPosition
    virtual void SetPosition(const glm::vec3& cameraPosition) override;

    virtual void SetViewport(Ref<Viewport> viewport) override;
    virtual const Viewport& GetViewport() override;

    void SetZoom(float zoom);  ///< Set the current zoom level
    float GetZoom();           ///< Get the current zoom level

    virtual void BeginPass() override;

   private:
    float m_Zoom;
    Ref<Viewport> m_Viewport;
    void RecalculatePerspectiveMatrix(float aspect);
    void RecalculateViewMatrix();
};

}  // namespace Blazar
