template <typename T, typename... Args>
void Entity::addComponent(Args&&... args) {
    m_entityManager->addComponent<T>(*this, std::forward<Args>(args)...);
}

template<typename T>
const T& Entity::getComponent() const {
    return m_entityManager->getComponent<T>(*this);
}

template <typename T>
T& Entity::getComponent() {
    // assert valid entity before returning
    return m_entityManager->getComponent<T>(*this);
}

template <typename T>
bool Entity::hasComponent() const {
    return m_entityManager->hasComponent<T>(*this);
}

template <typename T>
void Entity::removeComponent() {
    m_entityManager->removeComponent<T>(*this);
}