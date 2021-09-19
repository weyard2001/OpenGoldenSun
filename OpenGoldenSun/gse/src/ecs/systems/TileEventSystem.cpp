#include "gse/ecs/Scene.hpp"

#include "gse/ecs/components/TileEventComponent.hpp"
#include "gse/ecs/components/PlayerComponent.hpp"

#include "gse/ecs/systems/TileEventSystem.hpp"
#include "gse/ecs/systems/CollisionSystem.hpp"
#include "gse/ecs/systems/PlayerSystem.hpp"

using namespace gse;

TileEventSystem::TileEventSystem(Scene& scene, EventBus& eventBus) 
	: System(scene, eventBus, typeid(TileEventSystem)) {

	requireComponent<TileEventComponent>();
}

void TileEventSystem::update(const float& deltaTime) {
	const auto& playerSystem = getScene().getSystem<PlayerSystem>();
	const auto& playerSystemEntities = playerSystem.getEntities();

	std::vector<Entity> interactables;
	interactables.insert(interactables.end(), playerSystemEntities.begin(), playerSystemEntities.end());

	for (const auto& entity : interactables) {
		for (const auto& eventTileEntity : getEntities()) {
			const auto& entityCollider = entity.getComponent<CollisionComponent>().collider;
			const auto& eventTileComponent = eventTileEntity.getComponent<TileEventComponent>();
			const auto& eventTileType = eventTileComponent.eventType;

			const auto& eventTileRect = eventTileComponent.eventRect;

			switch (eventTileType) {
			case TileEventType::CollisionIndexChange:
				if (eventActive(entityCollider, eventTileRect)) {
					if (eventTileComponent.collisionIndex != getScene().getSystem<CollisionSystem>().getActiveCollisionIndex()) {
						// TODO: changing collision index should also change sprite z-indices.
						getEventBus().emit<TileEvent>(entity, eventTileComponent);
					}
				}

				break;
			case TileEventType::Jump:

				break;
			case TileEventType::Climb:

				break;
			case TileEventType::Slide:

				break;
			default:
				break;
			}
		}
	}
}

void TileEventSystem::entityAddedCallback(Entity entity) {
	
}

void TileEventSystem::entityRemovedCallback(Entity entity) {

}

bool TileEventSystem::eventActive(const std::unique_ptr<Collider>& entityCollider, const SDL_Rect& eventTileRect) {
	switch (entityCollider->type) {
	case ColliderType::Circle: {	// if entity has a circle collider, event should only trigger when center of circle touches either of the four edges of the event tile.
		const auto& entityCircleCollider = *(static_cast<CircleCollider*>(entityCollider.get()));
		auto circleCenterX = entityCircleCollider.pos.x + entityCircleCollider.radius;
		auto circleCenterY = entityCircleCollider.pos.y + entityCircleCollider.radius;

		auto tempX = circleCenterX, tempY = circleCenterY;
		const auto& leftEdge = eventTileRect.x;
		const auto rightEdge = leftEdge + eventTileRect.w;
		const auto& topEdge = eventTileRect.y;
		const auto bottomEdge = topEdge + eventTileRect.h;

		return circleCenterX >= leftEdge && circleCenterX <= rightEdge && circleCenterY >= topEdge && circleCenterY <= bottomEdge;
	}
	default:
		return false;
	}
}
