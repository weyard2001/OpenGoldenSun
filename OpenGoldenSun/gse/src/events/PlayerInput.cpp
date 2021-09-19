#include "utilities/Shapes.hpp"

#include "gse/events/PlayerInput.hpp"

#include "gse/ecs/components/PlayerComponent.hpp"
#include "gse/ecs/components/VelocityComponent.hpp"
#include "gse/ecs/components/CollisionComponent.hpp"

using namespace gse;

void PlayerInput::setPlayerEntity(Entity playerEntity) {
	m_playerEntity = playerEntity;
}

Entity PlayerInput::getPlayerEntity() const {
	return m_playerEntity;
}

const Uint8* PlayerInput::getKeyState() const {
	return m_keyState;
}

void PlayerInput::update() {
	// TODO: some frames should play longer than others
	// TODO: animation speed between different sprite frames should be different depending if player is idle, walking, or sprinting.
	auto& player = m_playerEntity.getComponent<PlayerComponent>();
	auto& playerVelocity = m_playerEntity.getComponent<VelocityComponent>();

	if (m_keyState[SDL_SCANCODE_W] && !m_keyState[SDL_SCANCODE_S]) {
		player.inputMask |= Input::Up;
	}
	else if (m_keyState[SDL_SCANCODE_S] && !m_keyState[SDL_SCANCODE_W]) {
		player.inputMask |= Input::Down;
	}
	if (m_keyState[SDL_SCANCODE_A] && !m_keyState[SDL_SCANCODE_D]) {
		player.inputMask |= Input::Left;
	}
	else if (m_keyState[SDL_SCANCODE_D] && !m_keyState[SDL_SCANCODE_A]) {
		player.inputMask |= Input::Right;
	}
	if (m_keyState[SDL_SCANCODE_LSHIFT]) {
		playerVelocity.sprintMultiplier = 1.5f;
	}

	if (!m_keyState[SDL_SCANCODE_W]) {
		player.inputMask &= ~Input::Up;
	}
	if (!m_keyState[SDL_SCANCODE_S]) {
		player.inputMask &= ~Input::Down;
	}
	if (!m_keyState[SDL_SCANCODE_A]) {
		player.inputMask &= ~Input::Left;
	}
	if (!m_keyState[SDL_SCANCODE_D]) {
		player.inputMask &= ~Input::Right;
	}
	if (!m_keyState[SDL_SCANCODE_LSHIFT]) {
		playerVelocity.sprintMultiplier = 1.f;
	}

	switch (player.inputMask) {
	case Input::Up:
		player.collisionStartDegree = 90.f;
		break;
	case Input::Up | Input::Left:
		player.collisionStartDegree = 135.f;
		break;
	case Input::Left:
		player.collisionStartDegree = 180.f;
		break;
	case Input::Left | Input::Down:
		player.collisionStartDegree = 225.f;
		break;
	case Input::Down:
		player.collisionStartDegree = 270.f;
		break;
	case Input::Down | Input::Right:
		player.collisionStartDegree = 315.f;
		break;
	case Input::Right:
		player.collisionStartDegree = 0.f;
		break;
	case Input::Right | Input::Up:
		player.collisionStartDegree = 45.f;
		break;
	case Input::None:
		break;
	}

	const auto& collision = m_playerEntity.getComponent<CollisionComponent>();
	const auto& circleCollider = *(static_cast<CircleCollider*>(collision.collider.get()));
	player.updateCollisionPoints(circleCollider);
}