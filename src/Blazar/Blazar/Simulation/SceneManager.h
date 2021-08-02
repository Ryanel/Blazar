#pragma once

#include "Blazar/Memory.h"
#include "Blazar/Time/Timestep.h"
#include "entt/entt.hpp"

#include "Scene.h"

namespace Blazar {

class Quad;

namespace Scenes {

/// A Scene is a collection of entities and systems that is run in the game loop.
/// Scenes are where calculation and gameplay happens.
/// More then one scene can be active at any one time.
class SceneManager {
   public:
    SceneManager();   ///< SceneManager Constructor
    ~SceneManager();  ///< SceneManager Destructor

    /// Updates the active scene by ts time.
    void update(Timestep& ts);

    /// Renders the active scene, using ts as the deltatime.
    void render(Timestep& ts);

    /// Sets the "main" scene
    void set_main_scene(Scene* scene) { m_currentScene.reset(scene); }

    /// Returns the current scene
    Ref<Scene> current() { return m_currentScene; }

   private:
    Ref<Scene> m_currentScene;
    Quad*      m_quad;
};
}  // namespace Scenes
}  // namespace Blazar
