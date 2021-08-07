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
    WorldViewer(Editor* editor);
    void render() override;

    entt::entity m_selected_entity = entt::null;

   private:
    bool isFocused;
    bool isTitleFocused;
};

}  // namespace Editor
}  // namespace Blazar
