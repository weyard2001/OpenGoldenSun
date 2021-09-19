#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <map>
#include <algorithm>

namespace gse {
	struct SpriteData final {
		SDL_Texture* texture = nullptr;
		int textureW = 0, textureH = 0;

		SDL_Rect rect = { 0, 0, 16, 16 };
		glm::ivec2 offset = { 0, 0 };
		std::int32_t spriteYBegin = 0;

		float scaleRelative = 1.f;
		bool dirty = false;	// true if animation hasn't finished playing turning animation and updated to new sprite direction
		// TODO: put animspeed for player turning in playercomponent?
	};

	struct SpriteComponent final {
		enum class Type {
			Background, Main // the order these sprite types are listed in determine the render order from lowest to highest.
		};

		std::map<Type, SpriteData> texturesInfo;

		SpriteComponent(std::map<Type, SpriteData> inTexturesInfo = {})
			: texturesInfo(inTexturesInfo) {

		}
	};
}