#pragma once

#include "Blazar/Renderer/Camera.h"

namespace Blazar {

class OrthographicCamera : public Camera {
   public:
    /// Creates an orthographic camera with a center X and Y position, extending width/2 and height/2 in each axis.
    OrthographicCamera(float x, float y, float zoom, float aspect);
    virtual void SetPosition(const glm::vec3& cameraPosition);

   private:
    void RecalculateViewMatrix();
};

}  // namespace Blazar
