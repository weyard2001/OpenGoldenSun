#include "gse/ecs/Scene.hpp"

using namespace gse;

Scene::Scene()
    : m_systemManager(*this),
    m_cameraEntity(createEntity()) {  // TODO: add a default level camera

}

Entity Scene::createEntity() {
    m_entitiesToAdd.push_back(m_entityManager.createEntity());
    return m_entitiesToAdd.back();
}

void Scene::destroyEntity(Entity entity) {
    m_entityManager.setEntityDestroyed(entity);
    m_entitiesToDestroy.push_back(entity);
}

void Scene::update(float deltaTime) {
    for (auto& entity : m_entitiesToDestroy) {
        m_systemManager.removeEntityFromSystems(entity);
        m_entityManager.destroyEntity(entity);
    }
    m_entitiesToDestroy.clear();

    for (auto& entity : m_entitiesToAdd) {
        m_systemManager.addEntityToSystems(entity);
    }
    m_entitiesToAdd.clear();

    m_systemManager.update(deltaTime);
}

void Scene::setActiveCameraEntity(Entity camera) {
    m_cameraEntity = camera;
}

EventBus& Scene::getEventBus() {
    return m_eventBus;
}

Entity Scene::getActiveCameraEntity() const {
    return m_cameraEntity;
}