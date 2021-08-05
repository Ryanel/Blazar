#pragma once

#include <unordered_map>

#include "Blazar/Assets/Resource.h"
#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {
class Texture2D;

namespace Editor {

class FilesystemViewer : public EditorWindow {
   public:
    FilesystemViewer(Editor* editor);
    void RenderItem(std::string name, std::string path, bool isDirectory, bool useList);

    void DrawTable();
    void DrawList();
    void DrawBreadCrumbs();
    void render() override;

    void NavigateUpFolder();
    void Refresh();

    bool options_open = false;
    bool dirty        = true;

    float m_size    = 96;
    float m_padding = 16;

    std::vector<std::string>                     m_path_heirarchy;
    std::unordered_map<std::string, std::string> m_current_directories;
    std::unordered_map<std::string, std::string> m_current_files;

    Ref<Resource<Texture2D>> m_texfolder;
    Ref<Resource<Texture2D>> m_texfile;
    Ref<Resource<Texture2D>> m_texrefresh;
    Ref<Resource<Texture2D>> m_texgear;

    bool m_optionEnableThumbnails = true;
    int  m_numthumbsCanLoad       = 1;
    int  m_optionThumbsCanLoad    = 1;
};

}  // namespace Editor
}  // namespace Blazar
