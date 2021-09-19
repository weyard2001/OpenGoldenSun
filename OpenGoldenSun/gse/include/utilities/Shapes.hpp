#pragma once

#include "gse/ecs/components/CollisionComponent.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <array>

namespace gse {
	namespace utils {
		float degToRadians(float degrees);

		void drawRect(SDL_Renderer* renderer, int x, int y, int w, int h);
		void drawFilledRect(SDL_Renderer* renderer, int x, int y, int w, int h);
		void drawCircle(SDL_Renderer* renderer, int radius, int centerX, int centerY);
		void drawTriangle(SDL_Renderer* renderer, std::array<glm::ivec2, 3u> vertices, int oppAndAdjLength, int x, int y);
	}
}