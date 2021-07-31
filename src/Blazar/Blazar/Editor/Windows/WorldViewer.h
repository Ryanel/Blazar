#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {
namespace Editor {

class WorldViewer : public EditorWindow {
   public:
    WorldViewer();
    void RenderWindow() override;

   private:
    bool                     isFocused;
    bool                     isTitleFocused;
    bool                     m_options_open;
    Ref<Resource<Texture2D>> m_texgear;
};

}  // namespace Editor
}  // namespace Blazar
