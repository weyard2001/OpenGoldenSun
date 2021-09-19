#pragma once

#include "gse/events/EventBus.hpp"
#include "gse/ecs/entities/Entity.hpp"
#include "gse/ecs/components/Component.hpp"
#include "gse/ecs/systems/System.hpp"

namespace gse {
	class Scene final {
	public:
		Scene();
		~Scene() = default;

		Entity createEntity();
		void destroyEntity(Entity entity);
		void update(float deltaTime);

		// Set the active Camera to be the one held by the input Entity.
		void setActiveCameraEntity(Entity camera);

		// Returns the Event Bus.
		EventBus& getEventBus();

		// Returns the Entity with the active Camera.
		Entity getActiveCameraEntity() const;

		template <typename T, typename... Args>
		void addSystem(Args&&... args);

		template <typename T>
		const T& getSystem() const;

		template <typename T>
		T& getSystem();

	private:
		EventBus m_eventBus;

		EntityManager m_entityManager;
		//ComponentManager m_componentManager;
		SystemManager m_systemManager;

		Entity m_cameraEntity;

		std::vector<Entity> m_entitiesToDestroy;
		std::vector<Entity> m_entitiesToAdd;
	};

#include "Scene.inl"
}