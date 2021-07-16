#pragma once

namespace Blazar {

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color() : r(0), g(0), b(0), a(1.0f) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

}  // namespace Blazar
