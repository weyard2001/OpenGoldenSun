#pragma once

#include "gse/ecs/systems/System.hpp"
#include "gse/ecs/components/PlayerComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <map>

namespace gse {
	struct VelocityComponent;

	class PlayerSystem final : public System {
	public:
		using PlayerStatus = std::pair<PlayerComponent::State, PlayerComponent::Direction>;

		PlayerSystem(Scene& scene, EventBus& eventBus);
		~PlayerSystem() = default;

		void update(const float& deltaTime) override;

	private:
		bool m_collided;

		std::unordered_map<std::uint8_t, PlayerComponent::Direction> m_directions;
		std::map<PlayerStatus, std::pair<glm::ivec2, std::vector<std::uint8_t>>> m_animationInfo;	// value.first = { spriteSheetSourceX, spriteSheetSourceY }, value.second = animation order
	};
}