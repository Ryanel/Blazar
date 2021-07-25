#define BLAZAR_CREATE_APPLICATION

#include "Blazar/Application.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Blazar.h"
#include "Blazar/Entry.h"
#include "Blazar/Utility/Path.h"
#include "Blazar/VFS/VFSFilesystem.h"
#include "Editor/Editor.h"
#include "Sandbox/Sandbox.h"
#include "Tracy.hpp"

using namespace Blazar;

class Game : public Blazar::Application {
   public:
    Game() {
        ZoneScoped;

        ResourceManager::Get()->m_vfs->add_mountpoint(new VFS::FileSystem("/Data/", "Contents/Data/", true));
        ResourceManager::Get()->m_vfs->add_mountpoint(new VFS::FileSystem("/Editor/", "Contents/Editor/", true));
        ResourceManager::Get()->m_vfs->refresh();

        Editor* editor = new Editor();
        PushLayer(editor);
        PushLayer(new Sandbox());

        editor->Setup();
    }

    ~Game() {}
};

namespace Blazar {

Application* CreateApplication() {
    tracy::SetThreadName("Main Thread");
    return new Game();
}

}  // namespace Blazar
