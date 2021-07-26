#include "bzpch.h"

#include <filesystem>
#include "VFSFilesystem.h"

namespace Blazar {

namespace VFS {

std::vector<std::string> FileSystem::files() {
    ZoneScoped;
    std::vector<std::string> list_of_files;

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(m_rPath)) {
        // Is it a file / directory?
        bool isNormalFile = is_regular_file(entry);
        std::string pathString = entry.path().generic_string();
        std::string relPathString = pathString.substr(m_rPath.size(), pathString.size() - m_rPath.size());
        std::string virtualPath = m_path + relPathString;
        if (isNormalFile) {
            list_of_files.push_back(virtualPath);
            auto& ptr = m_files.find(virtualPath);
            if (ptr == m_files.end()) {
                //LOG_CORE_TRACE("FileSystem: +++++ {} (at real path {})", virtualPath, entry.path());
                m_files.emplace(virtualPath, std::make_shared<FileSystem::FileInfo>(entry.path().string()));
            }
        }
    }

    // for (auto& path : list_of_files) { cache_file(path); }

    return list_of_files;
}

std::vector<std::byte> FileSystem::file_read(File* file) {
    ZoneScoped;
    auto ptr = get_file_info(file->path());
    if (ptr) {
        auto& val = ptr.value();
        if (!val->cached) { cache_file(file->path()); }
        return val->data;
    }
    return std::vector<std::byte>();
}

bool FileSystem::file_write(File* file, std::vector<std::byte> data) {
    ZoneScoped;
    auto ptr = get_file_info(file->path());
    throw;  // Not implemented
    if (ptr) {
        auto& val = ptr.value();
        if (!val->cached) { cache_file(file->path()); }
        return nullptr;
    }
    return false;
}

std::optional<std::shared_ptr<FileSystem::FileInfo>> FileSystem::get_file_info(std::string path) {
    auto& ptr = m_files.find(path);
    if (ptr != m_files.end()) { return std::optional<std::shared_ptr<FileSystem::FileInfo>>(ptr->second); }
    return std::nullopt;
}

void FileSystem::cache_file(std::string path) {
    ZoneScoped;
    auto ptr = get_file_info(path);
    if (!ptr) { throw; }

    auto& val = ptr.value();

    if (val->cached) { return; }
    val->cached = true;

    // Load data
    std::string newPath = val->realPath;
    std::filesystem::path fpath = std::filesystem::path(newPath);

    // Load the file in

    if (std::filesystem::exists(fpath)) {
        std::ifstream ifs(fpath, std::ios::binary | std::ios::ate);

        if (!ifs) throw std::runtime_error(fpath.string() + ": " + std::strerror(errno));

        auto end = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        auto size = std::size_t(end - ifs.tellg());
        val->size = size;

        if (size != 0) {
            val->data.resize(size);

            if (!ifs.read((char*)val->data.data(), val->data.size()))
                throw std::runtime_error(fpath.string() + ": " + std::strerror(errno));

            LOG_CORE_TRACE("FileSystem: CACHE {} ({} kb)", path, val->size / 1024);
        }
    } else {
        throw;  // Should not happen!
    }
}

size_t FileSystem::file_size(File* file) {
    auto ptr = get_file_info(file->path());
    if (ptr) {
        auto& val = ptr.value();
        if (!val->cached) { cache_file(file->path()); }
        return val->size;
    }
    return 0;
}

}  // namespace VFS
}  // namespace Blazar
