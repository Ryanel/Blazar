#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "entt/entt.hpp"

namespace Blazar {
namespace Editor {

class WorldViewer : public EditorWindow {
   public:
    WorldViewer();
    void render() override;

   private:
    entt::entity             m_selected_entity = entt::null;
    bool                     isFocused;
    bool                     isTitleFocused;
    bool                     m_options_open;
    bool                     m_opt_show_properties_always = true;
    Ref<Resource<Texture2D>> m_texgear;
};

}  // namespace Editor
}  // namespace Blazar
