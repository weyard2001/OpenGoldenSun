#pragma once

namespace gse {
	struct CameraComponent final {
		SDL_FRect view;
		SDL_Rect bounds;

		CameraComponent(float posX = 0.f, float posY = 0.f, float width = 0.f, float height = 0.f, SDL_Rect inBounds = {0, 0, 0, 0})
			: view({ posX, posY, width, height }),
			bounds(inBounds) {

		}
	};
}