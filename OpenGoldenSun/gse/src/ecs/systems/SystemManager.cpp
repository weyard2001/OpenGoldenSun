#include "gse/ecs/systems/System.hpp"

using namespace gse;

//SystemManager::SystemManager(ComponentManager& componentManager)
//    : m_componentManager(componentManager) {
//
//}

SystemManager::SystemManager(Scene& scene)
    : m_scene(scene) {

}

void SystemManager::addEntityToSystems(Entity entity) {
    const auto& entityMask = entity.getComponentMask();

    for (const auto& system : m_systems) {
        const auto& systemMask = system->getComponentMask();

        if ((entityMask & systemMask) == systemMask) {
            system->addEntity(entity);
        }
    }
}

void SystemManager::removeEntityFromSystems(Entity entity) {
    const auto& entityMask = entity.getComponentMask();

    for (const auto& system : m_systems) {
        const auto& systemMask = system->getComponentMask();

        if ((entityMask & systemMask) == systemMask) {
            system->removeEntity(entity);
        }
    }
}

void SystemManager::update(const float& deltaTime) {
    for (const auto& system : m_systems) {
        system->update(deltaTime);
    }
}
