#pragma once

#include "Blazar/Time/Timestep.h"
#include "entt/entt.hpp"

namespace Blazar {
namespace Scenes {

/// A Scene is a collection of entities and systems that is run in the game loop.
/// Scenes are where calculation and gameplay happens.
/// More then one scene can be active at any one time.
class Scene {
   public:
    Scene(std::string name);      ///< Scene Constructor
    virtual ~Scene();             ///< Scene Destructor
    entt::entity CreateEntity();  ///< Creates an entity within the scene

    /// Updates the scene by ts time.
    virtual void OnUpdate(Timestep& ts);

    /// Renders the scene, using ts as the deltatime.
    virtual void OnRender(Timestep& ts);

    /// Returns the Scene's Registry of Entities
    entt::registry& registry() { return m_registry; }

    /// Returns the Scene's Registry of Entities
    const entt::registry& registry() const { return m_registry; }

    // entt::handle CreateHandle(entt::entity ent); /// Creates an ease-of-use handle to use for this registry.

    size_t ticks() const;  ///< Returns the number of ticks that have been processed.

    std::string& name() { return m_name; }

   protected:
    size_t CalculateTicksThisFrame(Timestep& ts);  ///< Calculates the ticks to do this frame, from accrued time.
    size_t m_ticks = 0;                            ///< Number of ticks we've done

   private:
    float          m_time     = 0.0f;
    float          m_tickTime = 0.0f;
    float          m_tickRate = 20.0f;
    std::string    m_name;
    entt::registry m_registry;
};
}  // namespace Scenes
}  // namespace Blazar
