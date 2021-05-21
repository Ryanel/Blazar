#pragma once

#include "Blazar/Renderer/Buffer.h"

namespace Blazar {

/// Represents a viewport as part of a window, a rectangular slice.
class Viewport {
   public:
    float x, y, width, height;

    float GetAspect() const { return width / height; }

};

}  // namespace Blazar
