#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Layer/Layer.h"

using namespace Blazar;

namespace Blazar {
    class Texture2D;
}

class AssetEditorWindow : public Blazar::Layer {
   public:
    AssetEditorWindow() : Layer("Editor: Asset Window") {
        m_UpdatePath = LayerUpdatePath::ImGui;

        m_path_heirarchy.push_back("");
        m_path_heirarchy.push_back("Data");
        m_path_heirarchy.push_back("Shaders");

        m_texfolder = Resource<Texture2D>("/Editor/Textures/folder.png");
        m_texfile = Resource<Texture2D>("/Editor/Textures/file.png");
        m_texrefresh = Resource<Texture2D>("/Editor/Textures/refresh.png");
        m_texgear = Resource<Texture2D>("/Editor/Textures/gear.png");

        m_texfile.Load();
        m_texfolder.Load();
    }
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

    Resource<Texture2D> m_texfolder, m_texfile, m_texrefresh, m_texgear;
};
