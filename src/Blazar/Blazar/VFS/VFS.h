#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Blazar {
namespace VFS {

class File;
class VFS;

/// A mounted directory in the VFS tree.
class MountPoint {
   public:
    MountPoint(const std::string vpath, const std::string rpath) : m_path(vpath), m_rPath(rpath) {}
    const std::string& path() const { return m_path; }
    const std::string& real_path() const { return m_rPath; }
    virtual std::vector<std::string> files() = 0;

   protected:
    virtual std::vector<std::byte> file_read(File* file) = 0;
    virtual bool file_write(File* file, std::vector<std::byte> data) = 0;
    virtual size_t file_size(File* file) = 0;
    const std::string m_path;   ///< The virtual path things are mounted to
    const std::string m_rPath;  ///< The real path things come from

    friend class File;
};

/// A file in the VFS
class File {
   public:
    File(const std::string path, MountPoint* mountpoint) : m_path(path), m_mountpoint(mountpoint) {}
    std::vector<std::byte> read();
    bool write(std::vector<std::byte> data);

    MountPoint* mountpoint() { return m_mountpoint; }
    const std::string& path() const { return m_path; }
    size_t size();

   private:
    const std::string m_path;
    MountPoint* m_mountpoint;

   friend class VFS;
};

/// Represents a virtual filesystem constructed from several mountpoints. Usually only need one.
class VFS {
   public:
    void add_mountpoint(MountPoint* mp);
    void refresh();

    std::vector<std::byte> read(std::string vpath);
    std::optional<Ref<File>> get(std::string vpath);
    bool exists(std::string vpath);

    std::vector<std::string> filelist();

   private:
    std::unordered_map<std::string, Ref<File>> m_filelist;
    std::vector<MountPoint*> m_mountpoints;
};

}  // namespace VFS
}  // namespace Blazar
