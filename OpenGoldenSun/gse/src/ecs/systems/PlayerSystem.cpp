#include "gse/events/PlayerInput.hpp"

#include "gse/ecs/components/CollisionComponent.hpp"
#include "gse/ecs/components/SpriteComponent.hpp"
#include "gse/ecs/components/AnimationComponent.hpp"
#include "gse/ecs/components/VelocityComponent.hpp"

#include "gse/ecs/systems/PlayerSystem.hpp"

#include "utilities/Shapes.hpp"

using namespace gse;

PlayerSystem::PlayerSystem(Scene& scene, EventBus& eventBus)
	: System(scene, eventBus, typeid(PlayerSystem)),
	m_collided(false) {

	requireComponent<PlayerComponent>();

	m_directions[Input::Up] = PlayerComponent::Direction::Up;
	m_directions[Input::Down] = PlayerComponent::Direction::Down;
	m_directions[Input::Left] = PlayerComponent::Direction::Left;
	m_directions[Input::Right] = PlayerComponent::Direction::Right;
	m_directions[Input::Up | Input::Left] = PlayerComponent::Direction::UpLeft;
	m_directions[Input::Up | Input::Right] = PlayerComponent::Direction::UpRight;
	m_directions[Input::Down | Input::Left] = PlayerComponent::Direction::DownLeft;
	m_directions[Input::Down | Input::Right] = PlayerComponent::Direction::DownRight;

	// TODO: store each entity's sprite sheet info in a new data structure SpriteSheet, whose data is parsed from a .gseanim file
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::Up)] = std::make_pair(glm::ivec2({ 0, 4 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 4, 2, 3, 2, 4, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::Down)] = std::make_pair(glm::ivec2({ 0, 0 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 3, 2, 4, 2, 5, 2, 4, 2, 5, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::Left)] = std::make_pair(glm::ivec2({ 0, 2 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 3, 2, 4, 2, 5, 2, 4, 2, 5, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::Right)] = std::make_pair(glm::ivec2({ 0, 6 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 3, 2, 4, 2, 5, 2, 4, 2, 5, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::UpLeft)] = std::make_pair(glm::ivec2({ 0, 3 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 4, 2, 3, 2, 4, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::UpRight)] = std::make_pair(glm::ivec2({ 0, 5 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 4, 2, 3, 2, 4, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::DownLeft)] = std::make_pair(glm::ivec2({ 0, 1 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 3, 2, 4, 2, 5, 2, 4, 2, 5, 2, 1 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Idle, PlayerComponent::Direction::DownRight)] = std::make_pair(glm::ivec2({ 0, 7 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 2, 3, 2, 4, 2, 5, 2, 4, 2, 5, 2, 1 }));

	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::Up)] = std::make_pair(glm::ivec2({ 192, 4 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::Down)] = std::make_pair(glm::ivec2({ 192, 0 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::Left)] = std::make_pair(glm::ivec2({ 192, 2 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::Right)] = std::make_pair(glm::ivec2({ 192, 6 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::UpLeft)] = std::make_pair(glm::ivec2({ 192, 3 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::UpRight)] = std::make_pair(glm::ivec2({ 192, 5 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::DownLeft)] = std::make_pair(glm::ivec2({ 192, 1 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Walk, PlayerComponent::Direction::DownRight)] = std::make_pair(glm::ivec2({ 192, 7 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));

	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::Up)] = std::make_pair(glm::ivec2({ 384, 4 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::Down)] = std::make_pair(glm::ivec2({ 384, 0 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::Left)] = std::make_pair(glm::ivec2({ 384, 2 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::Right)] = std::make_pair(glm::ivec2({ 384, 6 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::UpLeft)] = std::make_pair(glm::ivec2({ 384, 3 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::UpRight)] = std::make_pair(glm::ivec2({ 384, 5 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::DownLeft)] = std::make_pair(glm::ivec2({ 384, 1 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
	m_animationInfo[std::make_pair(PlayerComponent::State::Sprint, PlayerComponent::Direction::DownRight)] = std::make_pair(glm::ivec2({ 384, 7 * 32 }), std::vector<std::uint8_t>({ 0, 1, 2, 3, 4, 5 }));
}

void PlayerSystem::update(const float& deltaTime) {
	for (auto& entity : getEntities()) {
		auto& player = entity.getComponent<PlayerComponent>();
		auto& playerSprite = entity.getComponent<SpriteComponent>();
		auto& playerAnimation = entity.getComponent<AnimationComponent>();
		auto& playerVelocity = entity.getComponent<VelocityComponent>();

		const auto& playerInputMask = player.inputMask;
		auto& playerCollisionStartDegree = player.collisionStartDegree;
		auto& multiplierX = playerVelocity.multiplier.x;
		auto& multiplierY = playerVelocity.multiplier.y;

		switch (playerInputMask) {
		case Input::Up:
			multiplierX = 0;
			multiplierY = -1;
			playerCollisionStartDegree = 90.f;
			break;
		case Input::Down:
			multiplierX = 0;
			multiplierY = 1;
			playerCollisionStartDegree = 270.f;
			break;
		case Input::Left:
			multiplierX = -1;
			multiplierY = 0;
			playerCollisionStartDegree = 180.f;
			break;
		case Input::Right:
			multiplierX = 1;
			multiplierY = 0;
			playerCollisionStartDegree = 0.f;
			break;
		case Input::Up | Input::Left:
			multiplierX = -1;
			multiplierY = -1;
			playerCollisionStartDegree = 135.f;
			break;
		case Input::Up | Input::Right:
			multiplierX = 1;
			multiplierY = -1;
			playerCollisionStartDegree = 45.f;
			break;
		case Input::Down | Input::Left:
			multiplierX = -1;
			multiplierY = 1;
			playerCollisionStartDegree = 225.f;
			break;
		case Input::Down | Input::Right:
			multiplierX = 1;
			multiplierY = 1;
			playerCollisionStartDegree = 315.f;
			break;
		case Input::None:
			multiplierX = 0;
			multiplierY = 0;
			player.state = PlayerComponent::State::Idle;
			break;
		}

		if (playerInputMask != Input::None) {
			player.direction = m_directions[playerInputMask];
			player.state = playerVelocity.sprintMultiplier > 1 ? PlayerComponent::State::Sprint : PlayerComponent::State::Walk;
		}

		const auto& curSpriteInfo = m_animationInfo[std::make_pair(player.state, player.direction)];
		auto& spriteData = playerSprite.texturesInfo[SpriteComponent::Type::Main];
		if ((player.oldState != player.state || player.oldDirection != player.direction) && !spriteData.dirty) {
			auto& spriteRect = spriteData.rect;
			spriteRect.x = curSpriteInfo.first.x;
			spriteData.spriteYBegin = curSpriteInfo.first.y;
			playerAnimation.frames = curSpriteInfo.second;
			playerAnimation.startFrameX = curSpriteInfo.first.x;
			playerAnimation.startFrameY = spriteRect.y;

			playerAnimation.curFrame = 0;
			playerAnimation.startTime = SDL_GetTicks();

			if (player.oldDirection != player.direction) {
				spriteData.dirty = true;
			}

			player.oldState = player.state;
			player.oldDirection = player.direction;
		}
	}
}
