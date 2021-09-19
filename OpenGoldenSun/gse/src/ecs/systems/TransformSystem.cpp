#include "gse/ecs/components/TransformComponent.hpp"
#include "gse/ecs/components/VelocityComponent.hpp"
#include "gse/ecs/components/CollisionComponent.hpp"
#include "gse/ecs/components/PlayerComponent.hpp"

#include "gse/ecs/systems/TransformSystem.hpp"

#include "gse/ecs/Scene.hpp"

using namespace gse;

TransformSystem::TransformSystem(Scene& scene, EventBus& eventBus)
    : System(scene, eventBus, typeid(TransformSystem)) {

    requireComponent<TransformComponent>();
}

void TransformSystem::update(const float& deltaTime) {
    for (auto& entity : getEntities()) {
        auto& entityTransform = entity.getComponent<TransformComponent>();

        if (entity.hasComponent<VelocityComponent>()) {
            const auto& entityVelocity = entity.getComponent<VelocityComponent>();
            const auto& multiplier = entityVelocity.multiplier;

            float magnitude = static_cast<float>(std::sqrt(multiplier.x * multiplier.x + multiplier.y * multiplier.y));
            auto multiplierNormalized = magnitude > 0 ? glm::vec2(multiplier.x / magnitude, multiplier.y / magnitude) : glm::vec2(0, 0);

            entityTransform.position.x += (entityVelocity.velocity.x * multiplierNormalized.x * entityVelocity.sprintMultiplier) * deltaTime;
            entityTransform.position.y += (entityVelocity.velocity.y * multiplierNormalized.y * entityVelocity.sprintMultiplier) * deltaTime;
        }

        if (entity.hasComponent<CollisionComponent>()) {
            auto& entityCollision = entity.getComponent<CollisionComponent>();
            auto& colliderPos = entityCollision.collider.get()->pos;
            auto newX = static_cast<int>(entityTransform.position.x * entityTransform.scale.x);
            auto newY = static_cast<int>(entityTransform.position.y * entityTransform.scale.y);

            entityCollision.collider.get()->updatePos(newX, newY);

            if (entity.hasComponent<PlayerComponent>() /* || entity.hasComponent<NPCComponent>() */) {
                const auto& circleCollider = *(static_cast<CircleCollider*>(entityCollision.collider.get()));
                auto& player = entity.getComponent<PlayerComponent>();
                player.updateCollisionPoints(circleCollider);
            }
        }
    }
}