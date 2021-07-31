#pragma once
#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {
namespace Components {
/// A RenderTransform is a transform relative to the current camera. The reason this is not just a normal transform is
/// due to the desire to allow for alternative coordinate schemes in the game code.
struct TextureComponent {
   Texture2D* tex;
};
}  // namespace Components
}  // namespace Blazar
