#include "gse/ecs/entities/Entity.hpp"

using namespace gse;

Entity::Entity(Entity::ID id, EntityManager* entityManager)
    : m_id(id),
    m_entityManager(entityManager) {

}

bool Entity::destroyed() const {
    return m_entityManager->entityDestroyed(*this);
}

const Entity::ID Entity::getID() const {
    return m_id;
}

const ComponentMask& Entity::getComponentMask() const {
    return m_entityManager->getComponentMask(*this);
}
