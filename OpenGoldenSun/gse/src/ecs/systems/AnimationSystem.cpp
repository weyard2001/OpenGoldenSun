#include "gse/ecs/systems/AnimationSystem.hpp"

#include "gse/ecs/components/SpriteComponent.hpp"
#include "gse/ecs/components/AnimationComponent.hpp"

using namespace gse;

AnimationSystem::AnimationSystem(Scene& scene, EventBus& eventBus)
	: System(scene, eventBus, typeid(AnimationSystem)) {

	requireComponent<SpriteComponent>();
	requireComponent<AnimationComponent>();
}

void AnimationSystem::update(const float& deltaTime) {
	for (auto& entity : getEntities()) {
		auto& sprite = entity.getComponent<SpriteComponent>();
		auto& animation = entity.getComponent<AnimationComponent>();
		auto elapsedTime = SDL_GetTicks() - animation.startTime;

		if (animation.frames.size() > 0) {
			// TODO: redo with passed in deltaTime so animationSpeed can be float
			auto& spriteData = sprite.texturesInfo[SpriteComponent::Type::Main];
			auto& spriteRect = spriteData.rect;
			animation.curFrame = (elapsedTime * animation.animationSpeed / 1000) % animation.frames.size();
			spriteRect.x = animation.startFrameX + (animation.frames[animation.curFrame] * spriteRect.w);

			if (spriteData.dirty) {
				auto diffA = abs(spriteData.spriteYBegin - spriteRect.y) / 32;
				auto diffB = 0;
				auto curY = ((elapsedTime * animation.animationSpeed * 5 / 1000) % 8) + 1;
				if (spriteData.spriteYBegin > spriteRect.y) {
					diffB = (spriteRect.y / 32) + (8 - spriteData.spriteYBegin / 32);

					if (diffA < diffB) {
						spriteRect.y = (animation.startFrameY + curY * 32) % spriteData.textureH;
					}
					else {
						spriteRect.y = (animation.startFrameY - curY * 32) % spriteData.textureH;
					}
				}
				else {
					diffB = (8 - spriteRect.y / 32) + (spriteData.spriteYBegin / 32);

					if (diffA <= diffB) {
						spriteRect.y = (animation.startFrameY - curY * 32) % spriteData.textureH;
					}
					else {
						spriteRect.y = (animation.startFrameY + curY * 32) % spriteData.textureH;
					}
				}

				if (spriteRect.y == spriteData.spriteYBegin) {
					spriteData.dirty = false;
				}
			}
		}
	}
}
