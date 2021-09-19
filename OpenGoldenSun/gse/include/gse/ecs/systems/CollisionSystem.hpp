#pragma once

#include "gse/ecs/components/CollisionComponent.hpp"

#include "gse/ecs/systems/System.hpp"

namespace gse {
    class CollisionSystem final : public System {
    public:
        //using Index = int;

        CollisionSystem(Scene& scene, EventBus& eventBus, SDL_Renderer* renderer);
        ~CollisionSystem() = default;

        void update(const float& deltaTime) override;

        // Set a new collision index that will be used for collision detection with entities.
        void setActiveCollisionIndex(int collisionIndex);

        // Get the current collision index being used for collision detection.
        const int& getActiveCollisionIndex() const;

        // Add a collider to the respective collision index.
        void insertCollider(std::size_t index, Entity entity);

        // Function callback for when player intersects with an event tile that changes the map's collision index.
        void tileEventCallback(TileEvent& tileEvent);

    protected:
        void entityAddedCallback(Entity entity) override;

    private:
        int m_activeCollisionIndex;
        std::vector<std::vector<Entity>> m_colliders;   // indices = collision index, elements = vector of colliders for that collision index

        SDL_Renderer* m_renderer;   // for collision debug
        std::array<float, 6u> m_glanceAngles;

        bool isCollidingWithPoint(const SDL_Point& point, Collider* collider) const;

        bool trianglePointColliding(const TriangleCollider& triangle, const SDL_Point& point) const;
        bool circlePointColliding(const CircleCollider& circle, const SDL_Point& point) const;
    };
}