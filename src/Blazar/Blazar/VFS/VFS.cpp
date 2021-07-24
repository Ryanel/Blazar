#include "bzpch.h"

#include "VFS.h"

namespace Blazar {
namespace VFS {

// File
std::vector<std::byte> File::read() { return this->m_mountpoint->file_read(this); }
bool File::write(std::vector<std::byte> data) { return this->m_mountpoint->file_write(this, data); }
void VFS::add_mountpoint(MountPoint* mp) { m_mountpoints.push_back(mp); }
size_t File::size() { return this->m_mountpoint->file_size(this); }

void VFS::refresh() {
    // Refresh the list.
    // For every mount point, check if the file exists. If so, set the mount point to the current one.
    // Otherwise, create file. If the file doesn't exist in ANY mount points, this means a deletion has occured. This is
    // not handled by the game engine yet
    // TODO: Fix deletions not working.
    for (auto* mp : m_mountpoints) {
        std::vector<std::string> files_in_mount = mp->files();
        for (auto& filepath : files_in_mount) {
            auto fp = m_filelist.find(filepath);
            if (fp != m_filelist.end()) {
                fp->second->m_mountpoint = mp;
            } else {
                m_filelist.emplace(filepath, std::make_shared<File>(filepath, mp));
            }
        }
    }
}

std::vector<std::byte> VFS::read(std::string vpath) {
    auto& it = m_filelist.find(vpath);
    if (m_filelist.find(vpath) != m_filelist.end()) {
        return it->second->read();
    } else {
        return std::vector<std::byte>();
    }
}

std::optional<Ref<File>> VFS::get(std::string vpath) {
    auto& it = m_filelist.find(vpath);
    if (m_filelist.find(vpath) != m_filelist.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}

bool VFS::exists(std::string vpath) {
    auto& it = m_filelist.find(vpath);
    return m_filelist.find(vpath) != m_filelist.end();
}

std::vector<std::string> VFS::filelist() {
    std::vector<std::string> list;
    for (auto& filepath : m_filelist) { list.push_back(filepath.first); }
    return list;
}

}  // namespace VFS
}  // namespace Blazar
