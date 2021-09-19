#include "gse/ecs/Scene.hpp"

#include "gse/ecs/components/TransformComponent.hpp"
#include "gse/ecs/components/RenderComponent.hpp"
#include "gse/ecs/components/SpriteComponent.hpp"
#include "gse/ecs/components/CameraComponent.hpp"
#include "gse/ecs/components/PlayerComponent.hpp"

#include "gse/ecs/systems/RenderSystem.hpp"
#include "gse/ecs/systems/CollisionSystem.hpp"

#include "utilities/Shapes.hpp"

#include <SDL2/SDL_image.h>

#include <algorithm>

#ifdef GSE_DEBUG
#include "gse/ecs/components/DebugComponent.hpp"
#endif // GSE_DEBUG


using namespace gse;

RenderSystem::RenderSystem(Scene& scene, EventBus& eventBus, SDL_Renderer* renderer)
	: System(scene, eventBus, typeid(RenderSystem)),
	m_renderer(renderer),
	m_sort(true) {

	requireComponent<TransformComponent>();
	requireComponent<RenderComponent>();
}

void RenderSystem::update(const float& deltaTime) {
	auto& entities = getEntities();

	if (m_sort) {
		std::sort(entities.begin(), entities.end(),
			[](const Entity& entityA, const Entity& entityB) {
				return entityA.getComponent<RenderComponent>().zIndex < entityB.getComponent<RenderComponent>().zIndex;
			});

		m_sort = false;
	}

	// draw the game sprites
	const auto& cameraView = getScene().getActiveCameraEntity().getComponent<CameraComponent>().view;
	for (auto& entity : entities) {
		if (!entity.hasComponent<SpriteComponent>()) {
			continue;
		}

		const auto& entityTransform = entity.getComponent<TransformComponent>();
		const auto& entitySprite = entity.getComponent<SpriteComponent>();
		for (const auto& entitySpriteTextureInfo : entitySprite.texturesInfo) {
			const auto& texture = entitySpriteTextureInfo.second.texture;
			const auto& sourceRect = entitySpriteTextureInfo.second.rect;
			const auto& entitySpriteData = entitySpriteTextureInfo.second;

			// TODO: sprite render also based on scaleRelative
			SDL_Rect destinationRect = {
				static_cast<int>(((entityTransform.position.x + entitySpriteData.offset.x) * entityTransform.scale.x) - cameraView.x),
				static_cast<int>(((entityTransform.position.y + entitySpriteData.offset.y) * entityTransform.scale.y) - cameraView.y),
				static_cast<int>(sourceRect.w * entityTransform.scale.x * entitySpriteData.scaleRelative),
				static_cast<int>(sourceRect.h * entityTransform.scale.y * entitySpriteData.scaleRelative) };
			SDL_RenderCopyEx(m_renderer, texture, &sourceRect, &destinationRect, entityTransform.rotation, nullptr, SDL_FLIP_NONE);
		}
	}

#ifdef GSE_DEBUG
	// and draw the debug info
	const auto& cameraViewX = static_cast<int>(cameraView.x);
	const auto& cameraViewY = static_cast<int>(cameraView.y);

	for (const auto& entity : entities) {
		const auto& debugComponent = entity.getComponent<DebugComponent>();
		const auto& debugColor = debugComponent.debugColor;
		SDL_SetRenderDrawColor(m_renderer, debugColor.r, debugColor.g, debugColor.b, debugColor.a);

		if (entity.hasComponent<CollisionComponent>()) {
			const auto& collision = entity.getComponent<CollisionComponent>();
			if (collision.collisionIndex != getScene().getSystem<CollisionSystem>().getActiveCollisionIndex() && collision.collisionIndex != -1) {
				continue;
			}

			const auto& colliderType = collision.collider.get()->type;
			switch (colliderType) {
			case ColliderType::Rectangle: {
				const auto& rectCollider = *(static_cast<RectangleCollider*>(collision.collider.get()));
				utils::drawRect(m_renderer, rectCollider.pos.x - cameraViewX, rectCollider.pos.y - cameraViewY, rectCollider.w, rectCollider.h);
				break;
			}
			case ColliderType::Circle: {
				const auto& circleCollider = *(static_cast<CircleCollider*>(collision.collider.get()));
				utils::drawCircle(m_renderer, circleCollider.radius, circleCollider.pos.x - cameraViewX + circleCollider.radius, circleCollider.pos.y - cameraViewY + circleCollider.radius);
				break;
			}
			case ColliderType::Triangle: {
				const auto& triangleCollider = *(static_cast<TriangleCollider*>(collision.collider.get()));
				//utils::drawTriangle(m_renderer, triangleCollider.vertices, triangleCollider.oppAndAdjLength, triangleCollider.pos.x - cameraViewX, triangleCollider.pos.y - cameraViewY);
				utils::drawTriangle(m_renderer, triangleCollider.vertices, triangleCollider.oppAndAdjLength, cameraViewX, cameraViewY);
				break;
			}
			default:
				break;
			}
		}

		if (entity.hasComponent<TileEventComponent>()) {
			const auto& eventTileRect = entity.getComponent<TileEventComponent>().eventRect;
			utils::drawRect(m_renderer, eventTileRect.x - cameraViewX, eventTileRect.y - cameraViewY, eventTileRect.w, eventTileRect.h);
		}

		if (entity.hasComponent<PlayerComponent>()) {
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);

			const auto& collisionPoints = entity.getComponent<PlayerComponent>().collisionPoints;
			for (const auto& point : collisionPoints) {
				utils::drawFilledRect(m_renderer, (point.x - 2) - cameraViewX, (point.y - 2) - cameraViewY, 4, 4);
			}
		}
	}
#endif // GSE_DEBUG
}

void RenderSystem::entityAddedCallback(Entity entity) {
	m_sort = true;
}

void RenderSystem::draw() {

}