#pragma once
#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "IComponent.h"

#include "misc/cpp/imgui_stdlib.h"

namespace Blazar {
namespace Components {
struct TextureComponent {
    Texture2D* tex;
    TextureComponent(Texture2D* tex) : tex(tex) {}
    TextureComponent(TextureComponent&& other) noexcept {
        this->tex = other.tex;
        other.tex = nullptr;
    }
    TextureComponent& operator=(TextureComponent&& other) noexcept {
        this->tex = other.tex;
        other.tex = nullptr;
        return *this;
    }
    void inspect() {
        ImGui::InputText("Texture", &this->tex->path, ImGuiInputTextFlags_ReadOnly);

        if (ImGui::BeginDragDropTarget()) {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("path");

            if (payload != nullptr) {
                if (payload->Preview) {
                    std::string path((const char*)payload->Data);
                    // TODO: Is this a texture?

                    this->tex = Texture2D::Load(path)->data();
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    REFLECTION_REFLECT(TextureComponent);
    REFLECTION_DESCRIPTION("Holds a reference to a texture");
    REFLECTION_MEMBER("tex");
    REFLECTION_METHOD("inspect");
    REFLECTION_END();
};
}  // namespace Components
}  // namespace Blazar
