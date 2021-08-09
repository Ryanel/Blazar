#pragma once

#include <optional>
#include <unordered_map>
#include <variant>

namespace Blazar {
// A file containing key-value pairs of information describing any kind of asset.
class MetaFile {
   public:
    using MetaProp = std::variant<std::string, bool, int, float>;
    using Key      = std::string;
    std::unordered_map<Key, MetaProp> m_properties;
    std::string                       m_path;

    void insert_or_assign(std::string key, MetaProp value) { m_properties.insert_or_assign(key, MetaProp(value)); }

    const MetaProp& get(std::string key) {
        const auto& itr = m_properties.find(key);
        if (itr != m_properties.end()) { return &itr->second; }
        return nullptr;
    }

    template<class T> std::optional<T> get(std::string key) {
        const auto& itr = m_properties.find(key);

        if (itr != m_properties.end()) {
            if (std::holds_alternative<T>(itr->second)) {
                return std::make_optional<T>(std::get<T>(itr->second));
            } else {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    void dump() {
        for (auto& x : m_properties) {
            std::visit([&x](auto&& e) { LOG_CORE_INFO("meta: {:20s} = {}", x.first, e); }, x.second);
        }
    }

    MetaProp& operator[](std::string& key) { return m_properties[key]; }
    MetaProp& operator[](const char* key) { return m_properties[key]; }

    const MetaProp& operator[](std::string& key) const { return (const MetaProp&)m_properties.at(key); }
};
}  // namespace Blazar
