#include "gse/ecs/systems/System.hpp"

#include <algorithm>

using namespace gse;

System::System(Scene& scene, EventBus& eventBus, ID id)
    : m_scene(scene),
    m_eventBus(eventBus),
    m_id(id) {

}

const System::ID System::getID() const {
    return m_id;
}

void System::addEntity(Entity entity) {
    m_entities.push_back(entity);
    entityAddedCallback(entity);
}

void System::removeEntity(Entity entity) {
    // "Erase-remove idiom" removes all matching entities and runs in O(n).
    // Better than using only erase, which is O(n^2), and z-index sorting will not be needed when entities are removed.
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
        [&entity](const Entity& other) {
            return entity == other;
        }), m_entities.end());
}

const std::vector<Entity>& System::getEntities() const {
    return m_entities;
}

std::vector<Entity>& System::getEntities() {
    return m_entities;
}

const ComponentMask& System::getComponentMask() const {
    return m_componentMask;
}

void System::entityAddedCallback(Entity entity) {

}

void System::entityRemovedCallback(Entity entity) {

}

Scene& System::getScene() {
    return m_scene;
}

EventBus& System::getEventBus() {
    return m_eventBus;
}
