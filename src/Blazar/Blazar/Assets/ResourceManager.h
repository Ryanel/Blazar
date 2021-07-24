#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "Resource.h"
#include "Blazar/VFS/VFS.h"

class AssetEditorWindow;
namespace Blazar {


/// Loads resources from disk
class ResourceManager {
   protected:
    ResourceManager(); ///< Constructor

   public:
    bool ReadFileFromPath(std::string_view path, std::vector<std::byte>& outBuffer);  ///< Loads a binary from the disk
    static ResourceManager* Get();  ///< Returns the current Resource Manager

    VFS::VFS* m_vfs;
   private:
    friend class AssetEditorWindow;
};
}  // namespace Blazar
