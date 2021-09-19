#pragma once

#include <glm/glm.hpp>

namespace gse {
	struct VelocityComponent final {
		glm::vec2 velocity;
		glm::ivec2 multiplier;

		float sprintMultiplier;

		VelocityComponent(glm::vec2 inVelocity = glm::vec2(40.f, 40.f), glm::ivec2 inMultiplier = glm::ivec2(0, 0), float inSprintMultiplier = 1.f)
			: velocity(inVelocity),
			multiplier(inMultiplier),
			sprintMultiplier(inSprintMultiplier) {

		}
	};
}