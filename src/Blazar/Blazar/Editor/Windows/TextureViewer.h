#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {
namespace Editor {

class TextureViewer : public EditorWindow {
   public:
    TextureViewer(Editor* editor, std::string image_path)
        : EditorWindow(editor, "Texture Viewer: " + image_path, "Texture Viewer", State::EDITOR_DOCKED),
          m_path(image_path) {
        m_expandRemainingSpace = true;
    }
    void render() override;
    void draw_image();

    Ref<Resource<Texture2D>> m_tex;
    std::string              m_path;
    bool                     loaded_resource = false;
    bool                     stayOpen        = true;
};

}  // namespace Editor
}  // namespace Blazar
