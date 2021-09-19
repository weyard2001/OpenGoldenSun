#pragma once

#include <SDL2/SDL.h>

namespace gse {
	struct DebugComponent final {
		SDL_Color debugColor;

		DebugComponent(SDL_Color inDebugColor = { 255, 0, 0, 255 })
			: debugColor(inDebugColor) {

		}
	};
}