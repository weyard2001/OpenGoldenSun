template <typename T, typename... Args>
void EntityManager::addComponent(Entity entity, Args&&... args) {
    const auto& componentID = Component<T>::getID();
    const auto entityID = entity.getID();

    // componentID should always be a valid index since we're limiting the component count
    // so we just need to check if there is anything in the pool yet, and if not, create the pool with an initial pool size
    if (!m_componentPools[componentID]) {
        m_componentPools[componentID] = std::make_unique<ComponentPool<T>>(m_poolSize);
    }

    auto& pool = *(dynamic_cast<ComponentPool<T>*>(m_componentPools[componentID].get()));
    if (entityID >= pool.size()) {
        pool.resize((entityID + 1) * 2);    // TODO: should limit the of entities on the map, otherwise pool will grow too large (pool size will be the same as number of entities on map, but not all of these entities will use this specific component being added)
    }

    T component(std::forward<Args>(args)...);

    pool[entityID] = std::move(component);
    m_componentMasks[entityID].set(componentID);
}

template<typename T>
const T& EntityManager::getComponent(Entity entity) const {
    const auto& componentID = Component<T>::getID();
    const auto entityID = entity.getID();

    const auto& pool = *(dynamic_cast<ComponentPool<T>*>(m_componentPools[componentID].get()));
    return pool[entityID];
}

template <typename T>
T& EntityManager::getComponent(Entity entity) {
    const auto& componentID = Component<T>::getID();
    const auto entityID = entity.getID();

    auto& pool = *(dynamic_cast<ComponentPool<T>*>(m_componentPools[componentID].get()));
    return pool[entityID];
}

template<typename T>
bool EntityManager::hasComponent(Entity entity) const {
    const auto& componentID = Component<T>::getID();
    const auto entityID = entity.getID();

    return m_componentMasks[entityID].test(componentID);
}

template <typename T>
void EntityManager::removeComponent(Entity entity) {
    const auto& componentID = Component<T>::getID();
    const auto entityID = entity.getID();

    m_componentMasks[entityID].set(componentID, 0);
}