#pragma once

#include <unordered_map>

#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Assets/Resource.h"
#include "Blazar/Layer/Layer.h"

using namespace Blazar;

namespace Blazar {
    class Texture2D;
}

class AssetEditorWindow : public Blazar::Layer {
   public:
    AssetEditorWindow();
    void RenderItem(std::string name, std::string path, bool isDirectory);
    void OnImGUIRender() override;

    void NavigateUpFolder();
    void Refresh();

    bool show = true;
    bool options_open = false;
    bool dirty = true;

    int m_size = 96;
    int m_padding = 16;

    std::vector<std::string> m_path_heirarchy;
    std::unordered_map<std::string, std::string> m_current_directories;
    std::unordered_map<std::string, std::string> m_current_files;

    Ref<Resource<Texture2D>> m_texfolder;
    Ref<Resource<Texture2D>> m_texfile;
    Ref<Resource<Texture2D>> m_texrefresh;
    Ref<Resource<Texture2D>> m_texgear;

    bool m_optionEnableThumbnails = true;
    int m_numthumbsCanLoad = 2;
    int m_optionThumbsCanLoad = 2;
};
