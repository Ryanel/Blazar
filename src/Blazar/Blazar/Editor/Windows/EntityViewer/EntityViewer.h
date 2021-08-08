#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Config.h"
#include "Blazar/Editor/EditorCommand.h"
#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Simulation/Entity.h"
#include "entt/entt.hpp"

namespace Blazar {
namespace Editor {

class EntityViewer : public EditorWindow {
   public:
    EntityViewer(Editor* editor, Entity e);
    void render() override;
    virtual void on_close();
   private:
    Entity m_entity;
    size_t m_footprint_size;
};




}  // namespace Editor
}  // namespace Blazar
