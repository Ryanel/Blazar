#pragma once

#include <string>

namespace Blazar {

namespace Utility {
class Paths {
   public:
    static std::string_view GetExtension(std::string& path) {
        size_t period_index = path.find(".", 0);

        if (period_index != std::string_view::npos) {
            std::string_view extension = path;
            extension = extension.substr(period_index + 1, extension.size() - period_index - 1);
            return extension;
        }
        return std::string_view();
    }
};
}  // namespace Utility

}  // namespace Blazar
