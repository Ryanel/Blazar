#pragma once
#include "VFS.h"

#include <optional>

namespace Blazar {
namespace VFS {
/// Mount point for a filesystem directory
class FileSystem : public MountPoint {
   public:
    FileSystem(std::string vpath, std::string rpath, bool enableWrite)
        : MountPoint(vpath, rpath), m_canWrite(enableWrite) {}

    // Inherited via MountPoint
    virtual std::vector<std::string> files() override;
    virtual std::vector<std::byte> file_read(File* file) override;
    virtual bool file_write(File* file, std::vector<std::byte> data) override;

   private:
    bool m_canWrite;

    struct FileInfo {
        bool cached;
        std::vector<std::byte> data;
        size_t size;
        std::string realPath;
        FileInfo(std::string realPath) : cached(false), data(std::vector<std::byte>()), size(0), realPath(realPath) {}
    };

    std::optional<std::shared_ptr<FileInfo>> get_file_info(std::string path);

    void cache_file(std::string path);
    std::unordered_map<std::string, std::shared_ptr<FileInfo>> m_files; // path -> info

    // Inherited via MountPoint
    virtual size_t file_size(File* file) override;
};
}  // namespace VFS
}  // namespace Blazar
