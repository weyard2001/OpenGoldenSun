template <typename T, typename... Args>
void Scene::addSystem(Args&&... args) {
	m_systemManager.addSystem<T>(std::forward<Args>(args)...);
}

template<typename T>
const T& Scene::getSystem() const {
	return m_systemManager.getSystem<T>();
}

template<typename T>
T& Scene::getSystem() {
	return m_systemManager.getSystem<T>();
}
