#include "gse/ecs/entities/Entity.hpp"

using namespace gse;

gse::EntityManager::EntityManager(std::uint32_t poolSize)
    : m_componentPools(MAX_COMPONENTS),
    m_poolSize(poolSize),
    m_entityCount(0) {
}

Entity EntityManager::createEntity() {
    Entity::ID entityID = 0;

    if (!m_freeIDs.empty()) {
        entityID = m_freeIDs.front();
        m_freeIDs.pop_front();
    }
    else {
        entityID = m_entityCount++;

        if (entityID >= m_componentMasks.size()) {
            m_componentMasks.resize((entityID + 1) * 2);
            m_entityDestroyed.resize(m_componentMasks.size());
        }
    }

    Entity entity(entityID, this);
    m_entityDestroyed[entityID] = false;

    return entity;
}

void EntityManager::destroyEntity(Entity entity) {
    const auto entityID = entity.getID();

    m_componentMasks[entityID].reset();
    m_freeIDs.push_back(entityID);

    for (auto& pool : m_componentPools) {
        if (pool) {
            pool->removeEntity(entityID);
        }
    }
}

bool EntityManager::entityDestroyed(Entity entity) const {
    return m_entityDestroyed[entity.getID()];
}

void EntityManager::setEntityDestroyed(Entity entity) {
    m_entityDestroyed[entity.getID()] = true;
}

const ComponentMask& gse::EntityManager::getComponentMask(Entity entity) const {
    return m_componentMasks[entity.getID()];
}