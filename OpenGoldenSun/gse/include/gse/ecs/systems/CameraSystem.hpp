#pragma once

#include "gse/ecs/systems/System.hpp"

namespace gse {
	class CameraSystem final : public System {
	public:
		CameraSystem(Scene& scene, EventBus& eventBus);

		void update(const float& deltaTime) override;

	private:

	};
}