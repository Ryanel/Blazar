#include "bzpch.h"

#include "Scene.h"

namespace Blazar {
namespace Scenes {

Scene::Scene(std::string name) : m_name(name) {}
Scene::~Scene() {}

void Scene::OnUpdate(Timestep& ts) { m_ticks += CalculateTicksThisFrame(ts); }
void Scene::OnRender(Timestep& ts) {}

entt::entity Scene::CreateEntity() { return m_registry.create(); }

size_t Scene::ticks() const
{
    return m_ticks;
}

size_t Scene::CalculateTicksThisFrame(Timestep& ts) {
    float tickDelta = 1.0f / m_tickRate;
    m_tickTime += ts;
    m_time += ts;

    size_t ticksToProcess = 0;

    while (m_tickTime > tickDelta) {
        ticksToProcess++;
        m_tickTime -= tickDelta;
    }

    return ticksToProcess;
}

}  // namespace Scenes
}  // namespace Blazar
