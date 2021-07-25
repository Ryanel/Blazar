#pragma once

namespace Blazar {

/// A 32-bit RGBA Color. 0-1 color space
struct Color {
    float r; ///< Red
    float g; ///< Green
    float b; ///< Blue
    float a; ///< ALpha

    /// Default constructor (black at full alpha)
    Color() : r(0), g(0), b(0), a(1.0f) {}

    /// Constructor (can leave alpha off, will default full alpha)
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

}  // namespace Blazar
