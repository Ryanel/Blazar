#include "bzpch.h"

#include <filesystem>
#include "ResourceManager.h"

namespace Blazar {

ResourceManager* g_resourceManager;

bool ResourceManager::GetBinaryFromDisk(std::string_view path, std::vector<char>& outBuffer) {
    ZoneScoped;
    // Check the filesystem if the file exists
    std::string newPath = "Contents/" + std::string(path);
    std::filesystem::path fpath = std::filesystem::path(newPath);

    // Load the file in

    if (std::filesystem::exists(fpath)) {
        std::ifstream ifs(fpath, std::ios::binary | std::ios::ate);

        if (!ifs) throw std::runtime_error(fpath.string() + ": " + std::strerror(errno));

        auto end = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        auto size = std::size_t(end - ifs.tellg());

        if (size == 0) { return false; }

        outBuffer.resize(size);

        if (!ifs.read((char*)outBuffer.data(), outBuffer.size()))
            throw std::runtime_error(fpath.string() + ": " + std::strerror(errno));

        return true;
    }

    LOG_CORE_WARN("fpath: {} exists: {}", fpath, std::filesystem::exists(fpath));

    return false;
}

ResourceManager* ResourceManager::Get() {
    if (g_resourceManager == nullptr) { g_resourceManager = new ResourceManager(); }
    return g_resourceManager;
}

}  // namespace Blazar
