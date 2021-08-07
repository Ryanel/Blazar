#pragma once

#include <typeinfo>
#include "Blazar/Component/NameComponent.h"
#include "Blazar/Config.h"
#include "Blazar/Log.h"
#include "Blazar/Time/Timestep.h"
#include "entt/entt.hpp"
#include "spdlog/fmt/fmt.h"

#include "imgui.h"

namespace Blazar {

// Maps Component IDS (COMPONENT_Name) to concrete types
// Also maps Component names to concrete types

enum class ComponentID : int { Name = 0, Transform = 1, Mesh = 2, Texture = 3 };

class ComponentRegistry {
   public:
    class TypeInfo {
       public:
        ComponentID type;
        std::string name;
        size_t      size;
    };
    static std::unordered_map<ComponentID, TypeInfo> id_to_type;

    // For ImGUI
    // Inspect if we can. Otherwise, fail via SFINAE

    template<typename T> class HasInspect {
       private:
        typedef char YesType[1];
        typedef char NoType[2];

        template<typename C> static YesType& test(decltype(&C::inspect));
        template<typename C> static NoType&  test(...);

       public:
        enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
    };

    // Has inspect
    template<typename T> typename std::enable_if<HasInspect<T>::value, void>::type component_inspect(T* obj) {
        obj->inspect();
    }

    // No inspect
    template<typename T> typename std::enable_if<std::is_class<T>::value, void>::type component_inspect(T* obj) {
        // No inspector here
        ImGui::Text("No inspector");
    }
};

}  // namespace Blazar
