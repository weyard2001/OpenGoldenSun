#include "utilities/Shapes.hpp"

#include "gse/ecs/components/CollisionComponent.hpp"

using namespace gse;

float utils::degToRadians(float degrees) {
	return static_cast<float>(degrees * M_PI / 180);
}

void utils::drawRect(SDL_Renderer* renderer, int x, int y, int w, int h) {
	auto collisionRect = SDL_Rect({ x, y, w, h });
	SDL_RenderDrawRect(renderer, &collisionRect);
}

void utils::drawFilledRect(SDL_Renderer* renderer, int x, int y, int w, int h) {
	auto filledRect = SDL_Rect({ x, y, w, h });
	SDL_RenderFillRect(renderer, &filledRect);
}

void utils::drawCircle(SDL_Renderer* renderer, int radius, int centerX, int centerY) {
	int d = 1 - radius;
	int x = 0;
	int y = radius;

	while (x < y) {
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

		++x;
		if (d < 0) {
			d += (2 * x) + 1;
		}
		else {
			--y;
			d += (2 * x) - (2 * y) + 1;
		}
	}
}

void utils::drawTriangle(SDL_Renderer* renderer, std::array<glm::ivec2, 3u> vertices, int oppAndAdjLength, int cameraX, int cameraY) {
	const auto& x1 = vertices[0].x - cameraX, x2 = vertices[1].x - cameraX, x3 = vertices[2].x - cameraX;
	const auto& y1 = vertices[0].y - cameraY, y2 = vertices[1].y - cameraY, y3 = vertices[2].y - cameraY;

	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLine(renderer, x1, y1, x3, y3);
}