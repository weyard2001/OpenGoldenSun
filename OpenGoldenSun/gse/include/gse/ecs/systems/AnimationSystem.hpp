#pragma once

#include "gse/ecs/systems/System.hpp"

namespace gse {
	class AnimationSystem final : public System {
	public:
		AnimationSystem(Scene& scene, EventBus& eventBus);

		void update(const float& deltaTime) override;

	private:

	};
}