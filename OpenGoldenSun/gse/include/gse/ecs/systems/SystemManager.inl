// TODO: add checks for valid systems (using hasSystem)

template <typename T, typename... Args>
void SystemManager::addSystem(Args&&... args) {
	// auto system = std::make_unique<T>(std::forward<Args>(args)...);
	m_systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));

	//auto& system = m_systems[typeid(T)];
	//system->setScene(m_scene);
}

template <typename T>
const T& SystemManager::getSystem() const {
	return getSystem<T>();
}

template<typename T>
T& SystemManager::getSystem() {
	auto retSystem = std::find_if(m_systems.begin(), m_systems.end(),
		[](const auto& system) {
			return system->getID() == typeid(T);
		});

	return *(dynamic_cast<T*>(retSystem->get()));
}

template <typename T>
bool SystemManager::hasSystem() const {
	System::ID systemID = typeid(T);

	for (const auto& system : m_systems) {
		if (systemID == system->getID()) {
			return true;
		}
	}

	return false;
	//return m_systems.find(typeid(T)) != m_systems.end();
}

template <typename T>
void SystemManager::removeSystem() {
	System::ID systemID = typeid(T);

	m_systems.erase(std::remove_if(m_systems.begin(), m_systems.end(),
		[&systemID](const auto& system) {
			return systemID == system->getID();
		}), m_systems.end());

	//auto system = m_systems.find(typeid(T));
	//m_systems.erase(system);
}