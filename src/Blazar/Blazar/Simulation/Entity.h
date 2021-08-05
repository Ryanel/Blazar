#pragma once

#include <typeinfo>
#include "Blazar/Component/NameComponent.h"
#include "Blazar/Config.h"
#include "Blazar/Log.h"
#include "Blazar/Time/Timestep.h"
#include "entt/entt.hpp"
#include "spdlog/fmt/fmt.h"

namespace Blazar {

/// Ease of convienence handle for an entity within a Scene
class Entity {
   public:
    Entity() {
        m_registry = nullptr;
        m_entity   = entt::null;
    }

    Entity(entt::registry* reg) {
        m_registry = reg;
        m_entity   = reg->create();
    }
    Entity(entt::registry* reg, entt::entity ent) {
        m_registry = reg;
        m_entity   = ent;
    }

    /// Destructor: Does not destroy the entity.
    ~Entity() {}

    /// Adds an already constructed component
    template<class T> void add(T& component) { m_registry->emplace<T>(m_entity, component); }
    /// Adds a constructed-inplace
    template<class T, typename... Args> void emplace(Args... args) {
#ifdef BLAZAR_CFG_ENTITY_CHECK_EMPLACEMENT
        if (has<T>()) {
            LOG_CORE_ERROR("Entity: {} attempt to add a second component {}", this->m_entity, typeid(T).name());
            return;
        }
#endif
        m_registry->emplace<T>(m_entity, args...);
    }
    /// Returns true if the entity has this component
    template<class T> bool has() const { return m_registry->all_of<T>(m_entity); }
    template<class T> T&   get() { return m_registry->get<T>(m_entity); }
    template<class T> T&   get() const { return m_registry->get<T>(m_entity); }

    /// Returns the underlying entity
    entt::entity& entity() { return m_entity; }
    /// Returns the owning registry
    entt::registry* registry() { return m_registry; }

    std::string name() const {
        std::string entityName;
        if (this->has<Components::NameComponent>()) {
            auto name  = get<Components::NameComponent>();
            entityName = name.name;
        } else {
            entityName = fmt::format("Entity {}", (int)m_entity);
        }
        return entityName;
    }

   private:
    entt::entity    m_entity;
    entt::registry* m_registry;
};

}  // namespace Blazar
