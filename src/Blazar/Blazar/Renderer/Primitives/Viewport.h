#pragma once

namespace Blazar {

/// Represents a viewport as part of a window, a rectangular slice.
class Viewport {
   public:
    float x, y, width, height;

    float GetAspect() const { return width / height; }

    Viewport() : x(0), y(0), width(0), height(0) {}
    Viewport(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    void Set(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

};

}  // namespace Blazar
