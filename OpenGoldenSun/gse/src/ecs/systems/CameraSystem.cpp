#include "gse/ecs/Scene.hpp"

#include "gse/ecs/systems/CameraSystem.hpp"

#include "gse/ecs/components/CameraComponent.hpp"
#include "gse/ecs/components/TransformComponent.hpp"
#include "gse/ecs/components/SpriteComponent.hpp"

using namespace gse;

CameraSystem::CameraSystem(Scene& scene, EventBus& eventBus)
	: System(scene, eventBus, typeid(CameraSystem)) {

	requireComponent<CameraComponent>();
	requireComponent<TransformComponent>();
}

void CameraSystem::update(const float& deltaTime) {
	for (auto& entity : getEntities()) {
		auto& camera = entity.getComponent<CameraComponent>().view;
		const auto& cameraBounds = entity.getComponent<CameraComponent>().bounds;
		const auto& entityTransform = entity.getComponent<TransformComponent>();
		const auto& entitySprite = entity.getComponent<SpriteComponent>();

		// TODO: scaling is involved, need to make sure zoom level does not make screen size bigger than map size * scale size.
		const auto& entitySpriteOffset = entitySprite.texturesInfo.at(SpriteComponent::Type::Main).offset;
		camera.x = ((entityTransform.position.x + 16 + entitySpriteOffset.x) * entityTransform.scale.x) - camera.w / 2;
		camera.y = ((entityTransform.position.y + 16 + entitySpriteOffset.y) * entityTransform.scale.y) - camera.h / 2;

		if (camera.x < 0) {
			camera.x = 0;
		}

		if (camera.y < 0) {
			camera.y = 0;
		}

		if (camera.x > cameraBounds.w * entityTransform.scale.x - camera.w) {
			camera.x = cameraBounds.w * entityTransform.scale.x - camera.w;
		}

		if (camera.y > cameraBounds.h * entityTransform.scale.y - camera.h) {
			camera.y = cameraBounds.h * entityTransform.scale.y - camera.h;
		}
	}
}
