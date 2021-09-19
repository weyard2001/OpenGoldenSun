#pragma once

#include "gse/events/EventBus.hpp"
#include "gse/ecs/components/Component.hpp"

namespace gse {
    class Scene;
    class Entity;

    class System {
    public:
        using ID = std::type_index;

        System(Scene& scene, EventBus& eventBus, ID id);
        virtual ~System() = default;

        const ID getID() const;

        // Adds an Entity to this System.
        void addEntity(Entity entity);

        // Removes an Entity from this System.
        void removeEntity(Entity entity);

        // Process incoming Event that this System is listening to.
        //virtual void processEvent(const Event& event);

        // Process all Entities and update their Component data each update.
        virtual void update(const float& deltaTime) = 0;

        // Returns a const reference to a vector containing all Entities in the System.
        const std::vector<Entity>& getEntities() const;

        // Returns a reference to a vector containing all Entities in the System.
        std::vector<Entity>& getEntities();

        // Returns a reference to this System's component mask.
        const ComponentMask& getComponentMask() const;

        // Set the bit corresponding to the Component required by this System.
        template <typename T>
        void requireComponent();

    protected:
        virtual void entityAddedCallback(Entity entity);
        virtual void entityRemovedCallback(Entity entity);

        Scene& getScene();
        EventBus& getEventBus();

    private:
        Scene& m_scene;
        EventBus& m_eventBus;
        ComponentMask m_componentMask;
        ID m_id;

        std::vector<Entity> m_entities;
    };

    class SystemManager final {
    public:
        //SystemManager(ComponentManager& componentManager);
        SystemManager(Scene& scene);
        ~SystemManager() = default;

        void addEntityToSystems(Entity entity);
        void removeEntityFromSystems(Entity entity);
        void update(const float& deltaTime);

        template <typename T, typename... Args>
        void addSystem(Args&&... args);

        template <typename T>
        const T& getSystem() const;

        template <typename T>
        T& getSystem();

        template <typename T>
        bool hasSystem() const;

        template <typename T>
        void removeSystem();

    private:
        Scene& m_scene;
        //ComponentManager& m_componentManager;
        std::vector<std::unique_ptr<System>> m_systems; // order of systems being processed needs to be maintained, use std::vector
    };

#include "System.inl"
#include "SystemManager.inl"
}