#pragma once

namespace Blazar {

/// Represents a viewport as part of a window, a rectangular slice.
class Viewport {
   public:
    float x;       ///< X position of viewport
    float y;       ///< Y position of viewport
    float width;   ///< Width of viewport
    float height;  ///< Height of viewport

    /// Aspect ratio of this viewport
    float GetAspect() const { return width / height; }

    /// Default constructor
    Viewport() : x(0), y(0), width(0), height(0) {}

    /// Constructs a viewport
    Viewport(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    /// Sets all the properties of this viewport at once
    void Set(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
};

}  // namespace Blazar
