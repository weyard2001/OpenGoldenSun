#pragma once

#include "gse/ecs/components/Component.hpp"
#include "gse/ecs/components/ComponentPool.hpp"

#include <cstddef>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <deque>
//#include <set>

// TODO: add checks for valid entities

namespace gse {
    class EntityManager;

    class Entity final {
    public:
        using ID = std::uint32_t;

        Entity() = default;
        Entity(ID id, EntityManager* entityManager = nullptr);
        virtual ~Entity() = default;

        bool destroyed() const;

        const ID getID() const;
        const ComponentMask& getComponentMask() const;

        template <typename T, typename... Args>
        void addComponent(Args&&... args);

        template <typename T>
        const T& getComponent() const;

        template <typename T>
        T& getComponent();

        template <typename T>
        bool hasComponent() const;

        template <typename T>
        void removeComponent();

        bool operator==(Entity other) const {
            return m_id == other.m_id;
        }

    private:
        EntityManager* m_entityManager; // cannot use reference since assignment operator= will be implicitly deleted
        ID m_id;

        friend class EntityManager;
    };

    class System;

    class EntityManager final {
    public:
        EntityManager(std::uint32_t poolSize = 128);
        ~EntityManager() = default;

        Entity createEntity();
        void destroyEntity(Entity entity);
        bool entityDestroyed(Entity entity) const;
        void setEntityDestroyed(Entity entity);
        const ComponentMask& getComponentMask(Entity entity) const;
        //void update();

        template <typename T, typename... Args>
        void addComponent(Entity entity, Args&&... args);

        template <typename T>
        const T& getComponent(Entity entity) const;

        template <typename T>
        T& getComponent(Entity entity);

        template <typename T>
        bool hasComponent(Entity entity) const;

        template <typename T>
        void removeComponent(Entity entity);

    private:
        std::vector<std::unique_ptr<IPool>> m_componentPools;    // vector indices = component IDs, pool indices = entity IDs
        std::vector<ComponentMask> m_componentMasks;            // vector indices = entity IDs
        std::vector<bool> m_entityDestroyed;
        std::deque<Entity::ID> m_freeIDs;

        //ComponentManager& m_componentManager;

        // TODO: Set max number of entities allowed in scene.
        Entity::ID m_entityCount;
        std::uint32_t m_poolSize;
    };

#include "Entity.inl"
#include "EntityManager.inl"
}